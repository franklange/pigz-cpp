#pragma once

#include <pigz-cpp/types.hpp>

#include <fstream>
#include <vector>

namespace pigzcpp {

auto flatten(std::vector<bytes_t>&) -> bytes_t;
auto slice(u64 max, u32 count) -> std::vector<segment_t>;

void append_move(bytes_t, bytes_t&);

} // namespace pigzcpp
