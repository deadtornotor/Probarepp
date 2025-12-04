#include "probare_core/data.hpp"

#include "probare_core/common.hpp"

#include <exception>
#include <iostream>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>
#include <utility>

namespace
{
    void run_test_entries(const ::probare::TestEntryVec &test_entries_,
                          bool parallel_);
}

namespace probare
{
    // Define the constructor after tests is known
    TestEntry::TestEntry(const std::string &name_,
                         std::function<void()> function_, bool serial_)
        : name(name_), fn(std::move(function_)), no_parallel(serial_)
    {
    }

    TestEntryPtr make_test(const std::string &name_,
                           std::function<void()> function_, bool serial_)
    {
        auto ptr = std::make_shared<TestEntry>(name_, function_, serial_);
        tests().emplace_back(ptr);
        return ptr;
    }

    TestEntryVec &tests()
    {
        static TestEntryVec vec;
        return vec;
    }
    TestStats &stats()
    {
        static TestStats stat{0, 0};
        return stat;
    }

    // Runs all tests sequentially (serial + parallel mixed)
    int run_tests(bool parallel_)
    {
        std::ostringstream oss;
        oss << "=== C++ Test Runner ===" << std::endl;
        oss << "Running " << tests().size() << " test(s)...";

        PROBARE_MESSAGE(oss)
        oss.clear();

        if (!parallel_) {
            run_test_entries(tests(), false);
        } else {
            TestEntryVec serial_tests;
            TestEntryVec parallel_tests;

            for (TestEntryPtr test : tests()) {
                if (test->no_parallel) {
                    serial_tests.emplace_back(test);
                } else {
                    parallel_tests.emplace_back(test);
                }
            }

            run_test_entries(serial_tests, false);
            run_test_entries(parallel_tests, true);
        }

        oss << std::endl << "=== Summary ===" << std::endl;
        oss << "[Passed] : " << stats().passed.load() << std::endl;
        oss << "[Failed] : " << stats().failed.load() << std::endl;

        PROBARE_MESSAGE(oss)
        oss.clear();

        return stats().failed.load();
    }
} // namespace probare

namespace
{
    using namespace probare;

    void run_test(TestEntryPtr test_entry_)
    {
        std::ostringstream oss;
        oss << "[ RUN ]       " << test_entry_->name;
        PROBARE_MESSAGE(oss);
        oss.clear();

        try {
            test_entry_->fn();
        } catch (const std::exception &ex) {
            oss << "[ EXCEPTION ] " << test_entry_->name
                << " threw: " << ex.what();
            PROBARE_ERROR(oss)
            _PROBARE_FAILED
        }
    }

    void run_test_entries(const TestEntryVec &test_entries_, bool parallel_)
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
        TestEntryQueue work_queue(
            TestEntryDeque(test_entries_.begin(), test_entries_.end()));

        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0)
            num_threads = 4; // Fallback if unknown

        std::vector<std::thread> workers;

        for (unsigned int i = 0; i < num_threads; ++i) {

            workers.emplace_back([&]() {
                try {
                    while (true) {
                        TestEntryPtr task = [&]() -> TestEntryPtr {
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
