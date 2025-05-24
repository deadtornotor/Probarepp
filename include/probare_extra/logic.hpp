#pragma once

#include "templates.hpp"

#define ASSERT_EQ(expected, actual)                                            \
    do {                                                                       \
        auto e = (expected);                                                   \
        auto a = (actual);                                                     \
        static_assert(                                                         \
            probare::IsStreamable<decltype(e)>::value,                         \
            "ASSERT_EQ: expected value is not streamable to std::ostream");    \
        static_assert(                                                         \
            probare::IsStreamable<decltype(a)>::value,                         \
            "ASSERT_EQ: actual value is not streamable to std::ostream");      \
        if (e != a) {                                                          \
            std::cerr << "ASSERT_EQ failed: " #expected " == " #actual         \
                      << "\n  Expected: " << e << "\n  Actual: " << a          \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl;   \
            probare::testing.failed.fetch_add(1);                              \
        } else {                                                               \
            probare::testing.passed.fetch_add(1);                              \
        }                                                                      \
    } while (0)

#define ASSERT_NEQ(expected, actual)                                           \
    do {                                                                       \
        auto e = (expected);                                                   \
        auto a = (actual);                                                     \
        static_assert(                                                         \
            probare::IsStreamable<decltype(e)>::value,                         \
            "ASSERT_NEQ: expected value is not streamable to std::ostream");   \
        static_assert(                                                         \
            probare::IsStreamable<decltype(a)>::value,                         \
            "ASSERT_NEQ: actual value is not streamable to std::ostream");     \
        if (e == a) {                                                          \
            std::cerr << "ASSERT_NEQ failed: " #expected " != " #actual        \
                      << "\n  Both were: " << a << " at " << __FILE__ << ":"   \
                      << __LINE__ << std::endl;                                \
            probare::testing.failed.fetch_add(1);                              \
        } else {                                                               \
            probare::testing.passed.fetch_add(1);                              \
        }                                                                      \
    } while (0)

#define ASSERT_GT(a, b)                                                        \
    do {                                                                       \
        if (!((a) > (b))) {                                                    \
            std::cerr << "ASSERT_GT failed: " #a " > " #b << "\n  a: " << a    \
                      << "\n  b: " << #b << " at " << __FILE__ << ":"          \
                      << __LINE__ << std::endl;                                \
            probare::testing.failed.fetch_add(1);                              \
        } else {                                                               \
            probare::testing.passed.fetch_add(1);                              \
        }                                                                      \
    } while (0)

#define ASSERT_LT(a, b)                                                        \
    do {                                                                       \
        if (!((a) < (b))) {                                                    \
            std::cerr << "ASSERT_LT failed: " #a " < " #b << "\n  a: " << #a   \
                      << "\n  b: " << #b << " at " << __FILE__ << ":"          \
                      << __LINE__ << std::endl;                                \
            probare::testing.failed.fetch_add(1);                              \
        } else {                                                               \
            probare::testing.passed.fetch_add(1);                              \
        }                                                                      \
    } while (0)

#define ASSERT_GT_EQ(a, b)                                                     \
    do {                                                                       \
        if (!((a) >= (b))) {                                                   \
            std::cerr << "ASSERT_GT_EQ failed: " #a " >= " #b                  \
                      << "\n  a: " << #a << "\n  b: " << #b << " at "          \
                      << __FILE__ << ":" << __LINE__ << std::endl;             \
            probare::testing.failed.fetch_add(1);                              \
        } else {                                                               \
            probare::testing.passed.fetch_add(1);                              \
        }                                                                      \
    } while (0)

#define ASSERT_LT_EQ(a, b)                                                     \
    do {                                                                       \
        if (!((a) <= (b))) {                                                   \
            std::cerr << "ASSERT_LT_EQ failed: " #a " <= " #b                  \
                      << "\n  a: " << #a << "\n  b: " << #b << " at "          \
                      << __FILE__ << ":" << __LINE__ << std::endl;             \
            probare::testing.failed.fetch_add(1);                              \
        } else {                                                               \
            probare::testing.passed.fetch_add(1);                              \
        }                                                                      \
    } while (0)
