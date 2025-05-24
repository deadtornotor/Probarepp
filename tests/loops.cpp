#include <probare>

TEST(self_test_lower)
{
    for (uint8_t i = 0; i < UINT8_MAX / 2; i++) {
        ASSERT_LT(i, i + 1);
        ASSERT_LT_EQ(i, i);
        ASSERT_LT_EQ(i, i + 1);
    }
}

TEST(self_test_greather)
{
    for (uint8_t i = 0; i < UINT8_MAX / 2; i++) {
        ASSERT_GT(i + 1, i);
        ASSERT_GT_EQ(i, i);
        ASSERT_GT_EQ(i + 1, i);
    }
}
