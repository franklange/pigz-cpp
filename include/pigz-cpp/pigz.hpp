#pragma once

#include <pigz-cpp/defaults.hpp>
#include <pigz-cpp/types.hpp>

namespace pigzcpp {

auto zip(const bytes_t&, u32 threads = defaults::num_threads) -> bytes_t;
auto zip(const path_t&,  u32 threads = defaults::num_threads, u64 chunk_size = defaults::chunk_size) -> bytes_t;

auto unzip(const bytes_t&) -> bytes_t;

} // namespace pigzcpp
