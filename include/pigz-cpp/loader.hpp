#pragma once

#include <pigz-cpp/defaults.hpp>
#include <pigz-cpp/types.hpp>

#include <istream>

namespace pigzcpp {

class loader_t
{
public:
    loader_t(std::istream&, segment_t, u64);

    auto next() -> bytes_t;

private:
    std::istream& stream;

    const segment_t segment;
    const u64 segment_size;

    u64 m_count{0};
    bytes_t m_buf;
};

} // namespace pigzcpp
