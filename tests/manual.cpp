#include <probare>

void my_manual_test();

namespace {
    inline auto my_manual_test_info = ::probare::make_test("my_manual_test", my_manual_test, true);
}

void my_manual_test(){
    ASSERT_TRUE(true);
}
