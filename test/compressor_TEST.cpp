#include <pigz-cpp/compressor.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace pigzcpp::test {

struct CompressorTest : public ::testing::Test
{
    compressor_t m_comp;

    const bytes_t m_header{0x78, 0x9c};
    const bytes_t m_bytes{0x01, 0x01, 0x01, 0x01, 0x01};

    auto is_zip(const bytes_t& bs) const -> bool
    {
        return (bytes_t{bs.begin(), bs.begin() + 2} == m_header);
    }
};

TEST_F(CompressorTest, bytes_0)
{
    const bytes_t bs;
    EXPECT_TRUE(m_comp.compress(bs).empty());
}

TEST_F(CompressorTest, bytes_1)
{
    const bytes_t bs{0x01};
    EXPECT_TRUE(is_zip(m_comp.compress(bs)));
}

TEST_F(CompressorTest, bytes_n)
{
    const auto r = m_comp.compress(m_bytes);

    EXPECT_TRUE(is_zip(r));
    EXPECT_LT(r.size(), m_bytes.size());
}

TEST_F(CompressorTest, span_0)
{
    const span_t s{m_bytes.begin(), m_bytes.begin()};
    EXPECT_TRUE(m_comp.compress(s).empty());
}

TEST_F(CompressorTest, span_1)
{
    const span_t s{m_bytes.begin(), m_bytes.begin() + 1};
    EXPECT_TRUE(is_zip(m_comp.compress(s)));
}

TEST_F(CompressorTest, span_n)
{
    const span_t s{m_bytes.begin(), m_bytes.end()};
    const auto r = m_comp.compress(s);

    EXPECT_TRUE(is_zip(r));
    EXPECT_LT(r.size(), m_bytes.size());
}

TEST_F(CompressorTest, compress_bytes)
{
    bytes_t res;
    compress_bytes(m_bytes, {0, 4}, res, true);

    EXPECT_TRUE(is_zip(res));
}

} // namespace pigzcpp::test
