#pragma once

#define ASSERT_THROW(statement, exception_type)                                \
    _PROBARE_SCOPE(                                                            \
        bool caught = false;                                                   \
        try { statement; } catch (const exception_type &) {                    \
            caught = true;                                                     \
        } catch (...) {} if (!caught) {                                        \
            std::ostringstream oss;                                            \
            oss << "ASSERT_THROW failed: " #statement                          \
                   " should throw " #exception_type                            \
                << " at " << __FILE__ << ":" << __LINE__;                      \
            PROBARE_ERROR(oss)                                                 \
            _PROBARE_FAILED                                                    \
        } else _PROBARE_PASSED)

#define ASSERT_NO_THROW(statement)                                             \
    _PROBARE_SCOPE(                                                            \
        try {                                                                  \
            statement;                                                         \
            _PROBARE_PASSED                                                    \
        } catch (...) {                                                        \
            std::ostringstream oss;                                            \
            oss << "ASSERT_NO_THROW failed: " #statement " should not throw"   \
                << " at " << __FILE__ << ":" << __LINE__;                      \
            PROBARE_ERROR(oss)                                                 \
            _PROBARE_FAILED                                                    \
        })
