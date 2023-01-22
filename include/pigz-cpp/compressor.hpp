#pragma once

#include <pigz-cpp/defaults.hpp>
#include <pigz-cpp/types.hpp>

#define ZLIB_CONST
#include <zlib.h>

namespace pigzcpp {

class compressor_t
{
public:
    compressor_t(u64 chunk_size = defaults::chunk_size);

    auto compress(const span_t) -> bytes_t;
    auto compress(const bytes_t&) -> bytes_t;

    auto finish_segment() -> bytes_t;
    auto finish_stream()  -> bytes_t;

private:
    auto compress(u32 mode) -> bytes_t;

    bytes_t m_buf;
    z_stream m_stream;
};

void compress_bytes(const bytes_t&, segment_t, bytes_t&, bool last);
void compress_file (const path_t&,  segment_t, u64 chunk_size, bytes_t&, bool last);

} // namespace pigzcpp
