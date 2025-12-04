#include "probare_core/data.hpp"
#include <probare>

TEST_NO_PARALLEL(serial_registration_verification)
{
    bool found = false;

    for (const auto &test : probare::tests()) {
        if (test->no_parallel) {
            if (test->name == "serial_registration_verification") {
                found = true;
            }
        }
    }

    ASSERT_TRUE(found);
}
