#pragma once

#include <atomic>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <vector>

namespace probare
{
    struct TestEntry : std::enable_shared_from_this<TestEntry> {
        std::string name;
        std::function<void()> fn;
        bool no_parallel;

        TestEntry(const std::string &name_, std::function<void()> function_,
                  bool serial_);
    };

    struct TestStats {
        std::atomic<int> passed{0};
        std::atomic<int> failed{0};

        inline void add_passed() { passed.fetch_add(1); }
        inline void add_failed() { failed.fetch_add(1); }
    };

    using TestEntryPtr = std::shared_ptr<TestEntry>;
    using TestEntryVec = std::vector<TestEntryPtr>;
    using TestEntryDeque = std::deque<TestEntryPtr>;
    using TestEntryQueue = std::queue<TestEntryPtr, TestEntryDeque>;

    TestEntryVec &tests();
    TestStats &stats();

    TestEntryPtr make_test(const std::string &name_,
                           std::function<void()> function_, bool serial_);

    int run_tests(bool parallel = true);
} // namespace probare
