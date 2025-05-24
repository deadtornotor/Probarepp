#pragma once

#define ASSERT_THROW(statement, exception_type)                                \
    do {                                                                       \
        bool caught = false;                                                   \
        try {                                                                  \
            statement;                                                         \
        } catch (const exception_type &) {                                     \
            caught = true;                                                     \
        } catch (...) {                                                        \
        }                                                                      \
        if (!caught) {                                                         \
            std::cerr << "ASSERT_THROW failed: " #statement                    \
                         " should throw " #exception_type                      \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl;   \
            probare::testing.failed.fetch_add(1);                              \
        } else {                                                               \
            probare::testing.passed.fetch_add(1);                              \
        }                                                                      \
    } while (0)

#define ASSERT_NO_THROW(statement)                                             \
    do {                                                                       \
        try {                                                                  \
            statement;                                                         \
            probare::testing.passed.fetch_add(1);                              \
        } catch (...) {                                                        \
            std::cerr << "ASSERT_NO_THROW failed: " #statement                 \
                         " should not throw"                                   \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl;   \
            probare::testing.failed.fetch_add(1);                              \
        }                                                                      \
    } while (0)
