#pragma once

#include "data.hpp"

// --- Test Registration Macros ---
#define TEST(name)                                                             \
    void name();                                                               \
    namespace                                                                  \
    {                                                                          \
        inline auto name##_registrar =                                         \
            ::probare::make_test(#name, &name, false);                         \
    }                                                                          \
    void name()

#define TEST_NO_PARALLEL(name)                                                 \
    void name();                                                               \
    namespace                                                                  \
    {                                                                          \
        inline auto name##_registrar =                                         \
            ::probare::make_test(#name, &name, true);                          \
    }                                                                          \
    void name()
