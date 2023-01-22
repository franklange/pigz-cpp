#include <pigz-cpp/loader.hpp>
#include <pigz-cpp/types.hpp>

#include <gtest/gtest.h>

#include <sstream>

namespace pigzcpp::test {

struct LoaderTest : public ::testing::Test
{
    std::stringstream m_stream{"this is a test!"};
    const u64 m_size{m_stream.str().size()};
};

TEST_F(LoaderTest, empty)
{
    std::stringstream s;
    loader_t loader{s, {0, 100}, 50};

    EXPECT_TRUE(loader.next().empty());
}

TEST_F(LoaderTest, chunk_size_0)
{
    std::stringstream s;
    loader_t loader{s, {0, 100}, 0};

    EXPECT_TRUE(loader.next().empty());
}

TEST_F(LoaderTest, segment_size_1)
{
    const bytes_t exp{0x69};
    loader_t l{m_stream, {2, 2}, 50};

    EXPECT_EQ(exp, l.next());
}

TEST_F(LoaderTest, segment_gt_stream)
{
    loader_t l{m_stream, {0, m_size + 10}, 50};
    EXPECT_EQ(m_stream.str().size(), l.next().size());
    EXPECT_TRUE(l.next().empty());
}

TEST_F(LoaderTest, normal)
{
    loader_t l{m_stream, {0, m_size - 1}, 4};

    for (u32 i = 0; i < 3; ++i)
        EXPECT_EQ(4, l.next().size());

    EXPECT_EQ(3, l.next().size());
}

} // namespace pigzcpp::test
