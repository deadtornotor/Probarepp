#include "probare_core/data.hpp"
#include <iostream>
#include <ostream>
#include <probare>

int main()
{
    // Start time
    auto start_time = std::chrono::high_resolution_clock::now();

    int error_value = probare::run_tests(true);

    // End time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    std::cout << "Duration: " << duration.count() << " ms" << std::endl;

    if (int failed = probare::testing.failed.load() != 0) {
        std::cout << "[ERROR] Had " << failed << " unexpected failing tests"
                  << std::endl;
    }

    if (int passed = probare::testing.passed.load() != 775) {
        std::cout << "[ERROR] Expected 775 tests to pass" << std::endl;
        std::cout << "[ERROR] Passed test " << passed << std::endl;
    }

    return error_value;
}

TEST(self_test_sample)
{
    // equal.hpp
    ASSERT_FLOAT_EQ(0.1 + 0.2, 0.3, 1e-9);
    ASSERT_STR_EQ("abc", std::string("abc"));
    ASSERT_MATCH("file_123.txt", "^file_\\d+\\.txt$");
    ASSERT_TRUE(true);
    ASSERT_FALSE(false);
    ASSERT_TRUE(!false);
    ASSERT_FALSE(!true);

    // logic.hpp
    ASSERT_EQ(5, 2 + 3);
    ASSERT_NEQ(5, 3 + 3);

    // throw.hpp
    ASSERT_THROW(throw std::runtime_error("oops"), std::runtime_error);
    ASSERT_NO_THROW(int x = 42; (void)x;);
}
