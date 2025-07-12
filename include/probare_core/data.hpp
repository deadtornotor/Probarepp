#pragma once

#include <atomic>
#include <cmath>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace probare
{
    struct TestEntry {
        std::string name;
        std::function<void()> fn;
        bool no_parallel;

        TestEntry(const std::string &name_, std::function<void()> function_,
                  bool serial_);
    };

    struct TestStats {
        std::atomic<int> passed{0};
        std::atomic<int> failed{0};
    };

    extern std::vector<TestEntry> tests;
    extern TestStats testing;

    int run_tests(bool parallel = true);
} // namespace probare
