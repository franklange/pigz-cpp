#include <pigz-cpp/types.hpp>
#include <pigz-cpp/utils.hpp>

#include <gtest/gtest.h>

namespace pigzcpp::test {

struct UtilsTest : public ::testing::Test
{
    const bytes_t m_bytes{0x01, 0x02, 0x03, 0x04, 0x05};
};

TEST_F(UtilsTest, flatten_0)
{
    std::vector<bytes_t> in;
    EXPECT_TRUE(flatten(in).empty());
}

TEST_F(UtilsTest, flatten_1)
{
    std::vector<bytes_t> in{m_bytes};
    const auto exp = 2 + ((m_bytes.size() - 2) * in.size());

    EXPECT_EQ(exp, flatten(in).size());
    EXPECT_TRUE(in.empty());
}

TEST_F(UtilsTest, flatten_n)
{
    std::vector<bytes_t> in{m_bytes, m_bytes, m_bytes};
    const auto exp = 2 + ((m_bytes.size() - 2) * in.size());

    EXPECT_EQ(exp, flatten(in).size());
    EXPECT_TRUE(in.empty());
}

TEST_F(UtilsTest, slice_empty)
{
    EXPECT_TRUE(slice(0, 5).empty());
}

TEST_F(UtilsTest, slice_count_0)
{
    EXPECT_TRUE(slice(5, 0).empty());
}

TEST_F(UtilsTest, slice_count_gt_max)
{
    EXPECT_THROW(slice(5, 10), std::runtime_error);
}

TEST_F(UtilsTest, slice_count_eq_max_1)
{
    const std::vector<segment_t> exp{{0, 0}};
    EXPECT_EQ(exp, slice(1, 1));
}

TEST_F(UtilsTest, slice_count_eq_max_n)
{
    const std::vector<segment_t> exp{{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}};
    EXPECT_EQ(exp, slice(5, 5));
}

TEST_F(UtilsTest, slice_even)
{
    const std::vector<segment_t> exp{{0, 1}, {2, 3}, {4, 5}};
    EXPECT_EQ(exp, slice(6, 3));
}

TEST_F(UtilsTest, slice_with_rest)
{
    const std::vector<segment_t> exp{{0, 1}, {2, 3}, {4, 6}};
    EXPECT_EQ(exp, slice(7, 3));
}

} // namespace pigzcpp::test

