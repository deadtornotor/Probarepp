#pragma once

#define ASSERT_TRUE(condition)                                                 \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "ASSERT_TRUE failed: " #condition << " at "           \
                      << __FILE__ << ":" << __LINE__ << std::endl;             \
            ++probare::testing.failed;                                         \
        } else {                                                               \
            ++probare::testing.passed;                                         \
        }                                                                      \
    } while (0)

#define ASSERT_FALSE(condition)                                                \
    do {                                                                       \
        if ((condition)) {                                                     \
            std::cerr << "ASSERT_FALSE failed: " #condition << " at "          \
                      << __FILE__ << ":" << __LINE__ << std::endl;             \
            ++probare::testing.failed;                                         \
        } else {                                                               \
            ++probare::testing.passed;                                         \
        }                                                                      \
    } while (0)

#define ASSERT_FLOAT_EQ(expected, actual, epsilon)                             \
    do {                                                                       \
        double e = (expected);                                                 \
        double a = (actual);                                                   \
        double diff = std::fabs(e - a);                                        \
        if (diff > (epsilon)) {                                                \
            std::cerr << "ASSERT_FLOAT_EQ failed: |" #expected " - " #actual   \
                         "| < " #epsilon                                       \
                      << "\n  Expected: " << e << "\n  Actual: " << a          \
                      << "\n  Diff: " << diff << " at " << __FILE__ << ":"     \
                      << __LINE__ << std::endl;                                \
            ++probare::testing.failed;                                         \
        } else {                                                               \
            ++probare::testing.passed;                                         \
        }                                                                      \
    } while (0)

#define ASSERT_STR_EQ(expected, actual)                                        \
    do {                                                                       \
        std::string e = (expected);                                            \
        std::string a = (actual);                                              \
        if (e != a) {                                                          \
            std::cerr << "ASSERT_STR_EQ failed: " #expected " == " #actual     \
                      << "\n  Expected: \"" << e << "\""                       \
                      << "\n  Actual: \"" << a << "\""                         \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl;   \
            ++probare::testing.failed;                                         \
        } else {                                                               \
            ++probare::testing.passed;                                         \
        }                                                                      \
    } while (0)

#define ASSERT_MATCH(str, pattern)                                             \
    do {                                                                       \
        std::string s = (str);                                                 \
        std::regex r(pattern);                                                 \
        if (!std::regex_match(s, r)) {                                         \
            std::cerr << "ASSERT_MATCH failed: \"" << s                        \
                      << "\" does not match regex \"" << #pattern << "\""      \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl;   \
            ++probare::testing.failed;                                         \
        } else {                                                               \
            ++probare::testing.passed;                                         \
        }                                                                      \
    } while (0)
