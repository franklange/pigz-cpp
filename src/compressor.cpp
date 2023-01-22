#include <pigz-cpp/compressor.hpp>
#include <pigz-cpp/loader.hpp>
#include <pigz-cpp/utils.hpp>

#include <fstream>
#include <stdexcept>

namespace pigzcpp {

compressor_t::compressor_t(const u64 chunk_size)
    : m_buf(chunk_size)
{
    m_stream.zalloc = Z_NULL;
    m_stream.zfree  = Z_NULL;
    m_stream.opaque = Z_NULL;
    deflateInit(&m_stream, Z_DEFAULT_COMPRESSION);
}

auto compressor_t::compress(const span_t in) -> bytes_t
{
    if (in.size() == 0)
        return {};

    m_stream.avail_in = in.size();
    m_stream.next_in  = reinterpret_cast<const Bytef*>(in.data());
    return compress(Z_NO_FLUSH);
}

auto compressor_t::compress(const bytes_t& in) -> bytes_t
{
    if (in.size() == 0)
        return {};

    m_stream.avail_in = in.size();
    m_stream.next_in  = reinterpret_cast<const Bytef*>(in.data());
    return compress(Z_NO_FLUSH);
}

auto compressor_t::finish_segment() -> bytes_t
{
    const auto res = compress(Z_SYNC_FLUSH);
    deflateEnd(&m_stream);
    return res;
}

auto compressor_t::finish_stream() -> bytes_t
{
    const auto res = compress(Z_FINISH);
    deflateEnd(&m_stream);
    return res;
}

auto compressor_t::compress(const u32 mode) -> bytes_t
{
    bytes_t res;

    do
    {
        m_stream.avail_out = m_buf.size();
        m_stream.next_out  = reinterpret_cast<Bytef*>(m_buf.data());

        deflate(&m_stream, mode);
        res.insert(res.end(), m_buf.begin(), m_buf.begin() + (m_buf.size() - m_stream.avail_out));
    }
    while (m_stream.avail_out == 0);

    return res;
}

void compress_file(const path_t& p, const segment_t segment, const u64 chunk_size, bytes_t& out, const bool last)
{
    std::ifstream file{p, std::ios::in | std::ios::binary};

    if (!file.is_open())
        throw std::runtime_error{"[pigzcpp][compress_thread] could not open file"};

    compressor_t c{chunk_size};
    loader_t loader{file, segment, chunk_size};

    while (true)
    {
        auto chunk = loader.next();
        if (chunk.empty())
            break;

        append_move(c.compress(std::move(chunk)), out);
    }

    append_move(last ? c.finish_stream() : c.finish_segment(), out);
}

void compress_bytes(const bytes_t& bs, const segment_t s, bytes_t& out, const bool last)
{
    compressor_t c;

    append_move(c.compress(span_t{(bs.begin() + s.begin), (bs.begin() + s.end + 1)}), out);
    append_move(last ? c.finish_stream() : c.finish_segment(), out);
}

} // namespace pigzcpp
