#include <pigz-cpp/compressor.hpp>
#include <pigz-cpp/pigz.hpp>

#include <gtest/gtest.h>

#include <fstream>

namespace pigzcpp::test {

namespace {

auto load(const path_t& p) -> bytes_t
{
    std::ifstream file{p, std::ios::in | std::ios::binary};

    if (!file.is_open())
        throw std::runtime_error{"[test] couldn't open test file"};

    bytes_t fbytes(std::filesystem::file_size(p));
    file.read(reinterpret_cast<char*>(fbytes.data()), fbytes.size());

    return fbytes;
}

} // namespace anonym

struct PigzcppTest : public ::testing::Test
{
    const bytes_t m_bytes{0x01, 0x01, 0x01, 0x01, 0x01};

    const path_t m_text_file{"files/test.txt"};
    const path_t m_zlib_file{"files/test.zlib"};
};

TEST_F(PigzcppTest, bytes_empty)
{
    const bytes_t bs;
    EXPECT_EQ(bs, unzip(zip(bs)));
}

TEST_F(PigzcppTest, bytes_threads_0)
{
    EXPECT_THROW(unzip(zip(bytes_t{}, 0)), std::runtime_error);
}

TEST_F(PigzcppTest, bytes_threads_1_chunksize_1)
{
    EXPECT_EQ(m_bytes, unzip(zip(m_bytes, 1)));
}

TEST_F(PigzcppTest, file_threads_0)
{
    EXPECT_THROW(unzip(zip(path_t{"foo"}, 0)), std::runtime_error);
}

TEST_F(PigzcppTest, file_chunksize_0)
{
    EXPECT_THROW(unzip(zip(path_t{"foo"}, 1, 0)), std::runtime_error);
}

TEST_F(PigzcppTest, unzip)
{
    std::cout << std::filesystem::current_path() << std::endl;
    EXPECT_EQ(load(m_text_file), unzip(load(m_zlib_file)));
}

TEST_F(PigzcppTest, file_threads_1)
{
    EXPECT_EQ(load(m_text_file), unzip(zip(m_text_file, 1)));
}

} // namespace pigzcpp::test
