#pragma once

#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>

namespace pigzcpp {

using u8  = std::uint8_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using path_t  = std::filesystem::path;

using bytes_t = std::vector<u8>;
using span_t  = std::span<const u8>;

// inclusive indices of a segment of a stream
struct segment_t
{
    u64 begin{0};
    u64 end{0};

    auto operator<=>(const segment_t&) const = default;
};

} // namespace pigzcpp
