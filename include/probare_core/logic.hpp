#pragma once

#include "common.hpp"

#define ASSERT_EQ(expected, actual)                                            \
    _PROBARE_SCOPE(                                                            \
        auto e = (expected); auto a = (actual); static_assert(                 \
            probare::IsStreamable<decltype(e)>::value,                         \
            "ASSERT_EQ: expected value is not streamable to std::ostream");    \
        static_assert(                                                         \
            probare::IsStreamable<decltype(a)>::value,                         \
            "ASSERT_EQ: actual value is not streamable to std::ostream");      \
        if (e != a) {                                                          \
            std::ostringstream oss;                                            \
            oss << "ASSERT_EQ failed: " #expected " == " #actual << std::endl  \
                << "  Expected: " << e << std::endl                            \
                << "  Actual: " << a << " at " << __FILE__ << ":" << __LINE__; \
            PROBARE_ERROR(oss)                                                 \
            _PROBARE_FAILED                                                    \
        } else _PROBARE_PASSED)

#define ASSERT_NEQ(expected, actual)                                           \
    _PROBARE_SCOPE(                                                            \
        auto e = (expected); auto a = (actual); static_assert(                 \
            probare::IsStreamable<decltype(e)>::value,                         \
            "ASSERT_NEQ: expected value is not streamable to std::ostream");   \
        static_assert(                                                         \
            probare::IsStreamable<decltype(a)>::value,                         \
            "ASSERT_NEQ: actual value is not streamable to std::ostream");     \
        if (e == a) {                                                          \
            std::ostringstream oss;                                            \
            oss << "ASSERT_NEQ failed: " #expected " != " #actual << std::endl \
                << "  Both were: " << a << " at " << __FILE__ << ":"           \
                << __LINE__;                                                   \
            PROBARE_ERROR(oss)                                                 \
            _PROBARE_FAILED                                                    \
        } else _PROBARE_PASSED)

#define ASSERT_GT(a, b)                                                        \
    _PROBARE_SCOPE(if (!((a) > (b))) {                                         \
        std::ostringstream oss;                                                \
        oss << "ASSERT_GT failed: " #a " > " #b << std::endl                   \
            << "  a: " << a << std::endl                                       \
            << "  b: " << #b << " at " << __FILE__ << ":" << __LINE__;         \
        PROBARE_ERROR(oss)                                                     \
        _PROBARE_FAILED                                                        \
    } else _PROBARE_PASSED)

#define ASSERT_LT(a, b)                                                        \
    _PROBARE_SCOPE(if (!((a) < (b))) {                                         \
        std::ostringstream oss;                                                \
        oss << "ASSERT_LT failed: " #a " < " #b << std::endl                   \
            << "  a: " << #a << std::endl                                      \
            << "  b: " << #b << " at " << __FILE__ << ":" << __LINE__;         \
        PROBARE_ERROR(oss)                                                     \
        _PROBARE_FAILED                                                        \
    } else _PROBARE_PASSED)

#define ASSERT_GT_EQ(a, b)                                                     \
    _PROBARE_SCOPE(if (!((a) >= (b))) {                                        \
        std::ostringstream oss;                                                \
        oss << "ASSERT_GT_EQ failed: " #a " >= " #b << std::endl               \
            << "  a: " << #a << std::endl                                      \
            << "  b: " << #b << " at " << __FILE__ << ":" << __LINE__;         \
        PROBARE_ERROR(oss)                                                     \
        _PROBARE_FAILED                                                        \
    } else _PROBARE_PASSED)

#define ASSERT_LT_EQ(a, b)                                                     \
    _PROBARE_SCOPE(if (!((a) <= (b))) {                                        \
        std::ostringstream oss;                                                \
        oss << "ASSERT_LT_EQ failed: " #a " <= " #b << std::endl               \
            << "  a: " << #a << std::endl                                      \
            << "  b: " << #b << " at " << __FILE__ << ":" << __LINE__;         \
        PROBARE_ERROR(oss)                                                     \
        _PROBARE_FAILED                                                        \
    } else _PROBARE_PASSED)
