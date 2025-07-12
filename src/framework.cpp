#include "probare_core/data.hpp"

#include "probare_core/common.hpp"

#include <exception>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <sstream>
#include <thread>
#include <utility>

namespace
{
    void run_test_entries(std::vector<probare::TestEntry> test_entries_,
                          bool parallel_);
}

namespace probare
{
    TestStats testing;

    // Define the constructor after tests is known
    TestEntry::TestEntry(const std::string &name_,
                         std::function<void()> function_, bool serial_)
        : name(name_), fn(std::move(function_)), no_parallel(serial_)
    {
        tests.emplace_back(*this); // Now valid — tests is in scope
    }

    // Runs all tests sequentially (serial + parallel mixed)
    int run_tests(bool parallel_)
    {
        std::ostringstream oss;
        oss << "=== Custom C++ Test Runner ===" << std::endl;
        oss << "Running " << tests.size() << " test(s)...";

        PROBARE_MESSAGE(oss)
        oss.clear();

        if (!parallel_) {
            run_test_entries(tests, false);
        } else {
            std::vector<TestEntry> serial_tests;
            std::vector<TestEntry> parallel_tests;

            for (const auto &test : tests) {
                if (test.no_parallel) {
                    serial_tests.push_back(test);
                } else {
                    parallel_tests.push_back(test);
                }
            }

            run_test_entries(serial_tests, false);
            run_test_entries(parallel_tests, true);
        }

        oss << std::endl << "=== Summary ===" << std::endl;
        oss << "✅ Passed: " << testing.passed.load() << std::endl;
        oss << "❌ Failed: " << testing.failed.load() << std::endl;

        PROBARE_MESSAGE(oss)
        oss.clear();

        return testing.failed.load();
    }
} // namespace probare

namespace
{
    using namespace probare;

    void run_test(const TestEntry &test_entry_)
    {
        std::ostringstream oss;
        oss << "[ RUN ]       " << test_entry_.name;
        PROBARE_MESSAGE(oss);
        oss.clear();

        try {
            test_entry_.fn();
        } catch (const std::exception &ex) {
            oss << "[ EXCEPTION ] " << test_entry_.name
                << " threw: " << ex.what();
            PROBARE_ERROR(oss)
            _PROBARE_FAILED
        }
    }

    void run_test_entries(std::vector<TestEntry> test_entries_, bool parallel_)
    {
        std::ostringstream oss;

        if (!parallel_) {
            oss << "Running " << test_entries_.size() << " serial test(s)...";
            PROBARE_MESSAGE(oss)
            oss.clear();

            for (const auto &test : test_entries_) {
                run_test(test);
            }
            return;
        }

        oss << "Running " << test_entries_.size() << " parallel test(s)...";
        PROBARE_MESSAGE(oss)
        oss.clear();

        std::mutex queue_mutex;
        std::queue<TestEntry, std::deque<TestEntry>> work_queue{
            std::deque<TestEntry>(test_entries_.begin(), test_entries_.end())};

        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0)
            num_threads = 4; // Fallback if unknown

        std::vector<std::thread> workers;

        for (unsigned int i = 0; i < num_threads; ++i) {

            workers.emplace_back([&]() {
                try {
                    while (true) {
                        TestEntry task = [&]() -> TestEntry {
                            std::lock_guard<std::mutex> lock(queue_mutex);
                            if (work_queue.empty())
                                throw std::runtime_error("done");
                            auto t = std::move(work_queue.front());
                            work_queue.pop();
                            return t;
                        }();

                        run_test(task);
                    }
                } catch (const std::runtime_error &e) {
                    if (std::string(e.what()) != "done")
                        throw; // rethrow if other runtime error
                               // else, normal thread exit when queue empty
                }
            });
        }

        for (auto &t : workers) {
            if (t.joinable())
                t.join();
        }
    }
} // namespace
