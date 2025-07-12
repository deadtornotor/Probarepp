#pragma once

#include "common.hpp"

#define ASSERT_TRUE(condition)                                                 \
    _PROBARE_SCOPE(if (!(condition)) {                                         \
        std::ostringstream oss;                                                \
        oss << "ASSERT_TRUE failed: " #condition << " at " << __FILE__ << ":"  \
            << __LINE__;                                                       \
        PROBARE_ERROR(oss)                                                     \
        _PROBARE_FAILED;                                                       \
    } else _PROBARE_PASSED)

#define ASSERT_FALSE(condition)                                                \
    _PROBARE_SCOPE(if ((condition)) {                                          \
        std::ostringstream oss;                                                \
        oss << "ASSERT_FALSE failed: " #condition << " at " << __FILE__ << ":" \
            << __LINE__;                                                       \
        PROBARE_ERROR(oss)                                                     \
        _PROBARE_FAILED;                                                       \
    } else _PROBARE_PASSED)

#define ASSERT_FLOAT_EQ(expected, actual, epsilon)                             \
    _PROBARE_SCOPE(double e = (expected); double a = (actual);                 \
                   double diff = std::fabs(e - a); if (diff > (epsilon)) {     \
                       std::ostringstream oss;                                 \
                       oss << "ASSERT_FLOAT_EQ failed: |" #expected            \
                              " - " #actual "| < " #epsilon                    \
                           << std::endl                                        \
                           << "  Expected: " << e << std::endl                 \
                           << "  Actual: " << a << std::endl                   \
                           << "  Diff: " << diff << " at " << __FILE__ << ":"  \
                           << __LINE__;                                        \
                       PROBARE_ERROR(oss)                                      \
                       _PROBARE_FAILED;                                        \
                   } else _PROBARE_PASSED)

#define ASSERT_STR_EQ(expected, actual)                                        \
    _PROBARE_SCOPE(                                                            \
        std::string e = (expected); std::string a = (actual); if (e != a) {    \
            std::ostringstream oss;                                            \
            oss << "ASSERT_STR_EQ failed: " #expected " == " #actual           \
                << std::endl                                                   \
                << "  Expected: \"" << e << "\"" << std::endl                  \
                << "  Actual: \"" << a << "\""                                 \
                << " at " << __FILE__ << ":" << __LINE__;                      \
            PROBARE_ERROR(oss)                                                 \
            _PROBARE_FAILED;                                                   \
        } else _PROBARE_PASSED)

#define ASSERT_MATCH(str, pattern)                                             \
    _PROBARE_SCOPE(std::string s = (str); std::regex r(pattern);               \
                   if (!std::regex_match(s, r)) {                              \
                       std::ostringstream oss;                                 \
                       oss << "ASSERT_MATCH failed: \"" << s                   \
                           << "\" does not match regex \"" << #pattern << "\"" \
                           << " at " << __FILE__ << ":" << __LINE__;           \
                       PROBARE_ERROR(oss)                                      \
                       _PROBARE_FAILED;                                        \
                   } else _PROBARE_PASSED)
