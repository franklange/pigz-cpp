#include <pigz-cpp/loader.hpp>

namespace pigzcpp {

loader_t::loader_t(std::istream& s, segment_t segm, const u64 chunk_size)
    : stream{s}
    , segment{std::move(segm)}
    , segment_size{(segment.begin == segment.end) ? 1 : ((segment.end - segment.begin) + 1)}
    , m_buf(chunk_size)
{
    stream.seekg(segment.begin);
}

auto loader_t::next() -> bytes_t
{
    if (m_buf.size() == 0)
        return {};

    if (m_count == segment_size)
        return {};

    stream.read(reinterpret_cast<char*>(m_buf.data()), std::min(m_buf.size(), (segment_size - m_count)));
    m_count += stream.gcount();

    return {m_buf.begin(), m_buf.begin() + stream.gcount()};
}

} // namespace pigzcpp
