#pragma once

#include "data.hpp"

// --- Test Registration Macros ---
#define TEST(name)                                                             \
    void name();                                                               \
    static probare::TestEntry name##_test_entry(#name, name, false);           \
    void name()

#define TEST_NO_PARALLEL(name)                                                 \
    void name();                                                               \
    static probare::TestEntry name##_test_entry(#name, name, true);            \
    void name()
