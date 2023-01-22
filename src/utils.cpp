#include <pigz-cpp/utils.hpp>

#include <iterator>
#include <numeric>
#include <stdexcept>

namespace pigzcpp {

auto flatten(std::vector<bytes_t>& segments) -> bytes_t
{
    if (segments.empty())
        return {};

    // Add header for final result and skip it for each segment
    bytes_t res{0x78, 0x9c};

    for (auto& s : segments)
    {
        res.insert(res.end(), s.begin() + 2, s.end());
        s.clear();
    }
    segments.clear();

    return res;
}

auto slice(const u64 max, const u32 count) -> std::vector<segment_t>
{
    if (!max || !count)
        return {};

    if (count > max)
        throw std::runtime_error{"[pigzcpp][slice] more slices than elements"};

    std::vector<segment_t> res;
    const auto size = max / count;

    for (u32 i = 0; i < (count - 1); ++i)
    {
        const auto offset = i * size;
        res.push_back({offset, (offset + size) - 1});
    }
    res.push_back({size * (count - 1), max - 1});

    return res;
}

void append_move(bytes_t chunk, bytes_t& out)
{
    out.insert(out.end(), std::make_move_iterator(chunk.begin()), std::make_move_iterator(chunk.end()));
}

} // namespace pigzcpp
