#include "probare_core/equal.hpp"
#include "probare_core/logic.hpp"
#include "probare_core/test.hpp"
#include "probare_core/throw.hpp"
#include <probare>
#include <stdexcept>

// just to add some tests to make sure its scalable
TEST(dummy_test_0)
{
    ASSERT_FALSE(false);
}
TEST(dummy_test_1)
{
    ASSERT_TRUE(true);
}
TEST(dummy_test_2)
{
    ASSERT_EQ(true, true);
}
TEST(dummy_test_3)
{
    ASSERT_EQ(false, false);
}
TEST(dummy_test_4)
{
    ASSERT_NEQ(true, false);
}
TEST(dummy_test_5)
{
    ASSERT_NEQ(false, true);
}
TEST(dummy_test_6)
{
    ASSERT_GT(6, 5);
}
TEST(dummy_test_7)
{
    ASSERT_LT(5, 6);
}
TEST(dummy_test_8)
{
    ASSERT_NO_THROW({});
}
TEST(dummy_test_9)
{
    ASSERT_THROW({ throw std::runtime_error(""); }, std::runtime_error);
}
TEST(dummy_test_10)
{
    ASSERT_TRUE(false == false);
}
TEST(dummy_test_11)
{
    ASSERT_STR_EQ("1", "1");
}
