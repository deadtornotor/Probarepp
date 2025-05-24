#include "probare_extra/data.hpp"

#include <iostream>
#include <queue>
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
        std::cout << "=== Custom C++ Test Runner ===" << std::endl;
        std::cout << "Running " << tests.size() << " test(s)...\n\n";
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

        std::cout << "\n=== Summary ===" << std::endl;
        std::cout << "✅ Passed: " << testing.passed.load() << std::endl;
        std::cout << "❌ Failed: " << testing.failed.load() << std::endl;

        return testing.failed.load();
    }
} // namespace probare

namespace
{
    using namespace probare;

    void run_test(const TestEntry &test_entry_)
    {
        try {
            test_entry_.fn();
        } catch (const std::exception &ex) {
            std::cerr << "[ EXCEPTION ] " << test_entry_.name
                      << " threw: " << ex.what() << std::endl;
            testing.failed.fetch_add(1);
        } catch (...) {
            std::cerr << "[ EXCEPTION ] " << test_entry_.name
                      << " threw an unknown exception" << std::endl;
            testing.failed.fetch_add(1);
        }
    }

    void run_test_mutex(const TestEntry &test_entry_, std::mutex &output_mutex_)
    {
        {
            std::lock_guard<std::mutex> out_lock(output_mutex_);
            std::cout << "[ RUN ]       " << test_entry_.name << std::endl;
        }

        try {
            test_entry_.fn();
        } catch (const std::exception &ex) {
            std::lock_guard<std::mutex> lock(output_mutex_);
            std::cerr << "[ EXCEPTION ] " << test_entry_.name
                      << " threw: " << ex.what() << std::endl;
            testing.failed.fetch_add(1);
        } catch (...) {
            std::lock_guard<std::mutex> lock(output_mutex_);
            std::cerr << "[ EXCEPTION ] " << test_entry_.name
                      << " threw an unknown exception" << std::endl;
            testing.failed.fetch_add(1);
        }
    }

    void run_test_entries(std::vector<TestEntry> test_entries_, bool parallel_)
    {
        if (!parallel_) {
            std::cout << "Running " << test_entries_.size()
                      << " serial test(s)...\n";
            for (const auto &test : test_entries_) {
                std::cout << "[ RUN ]       " << test.name << std::endl;
                run_test(test);
            }
            return;
        }
        std::cout << "Running " << test_entries_.size()
                  << " parallel test(s)...\n";

        std::mutex output_mutex;
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

                        run_test_mutex(task, output_mutex);
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
