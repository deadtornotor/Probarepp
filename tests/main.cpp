#include <probare_entry>

TEST(self_test_sample)
{
    // equal.hpp
    ASSERT_FLOAT_EQ(0.1 + 0.2, 0.3, 1e-9);
    ASSERT_STR_EQ("abc", std::string("abc"));
    ASSERT_MATCH("file_123.txt", "^file_\\d+\\.txt$");
    ASSERT_TRUE(true);
    ASSERT_FALSE(false);
    ASSERT_TRUE(!false);
    ASSERT_FALSE(!true);

    // logic.hpp
    ASSERT_EQ(5, 2 + 3);
    ASSERT_NEQ(5, 3 + 3);

    // throw.hpp
    ASSERT_THROW(throw std::runtime_error("oops"), std::runtime_error);
    ASSERT_NO_THROW(int x = 42; (void)x;);
}
