#include <pigz-cpp/compressor.hpp>
#include <pigz-cpp/pigz.hpp>
#include <pigz-cpp/utils.hpp>

#include <zlib.h>

#include <fstream>
#include <functional>
#include <stdexcept>
#include <vector>
#include <thread>

namespace pigzcpp {

namespace {

using thread_func = std::function<void(u32)>;

void fan_out(const u32 num_threads, const thread_func f)
{
    std::vector<std::thread> threads;

    for (u32 i = 0; i < num_threads; ++i)
        threads.push_back(std::thread{f, i});

    for (auto& t : threads)
        if (t.joinable()) t.join();
}

} // namespace anonym

auto zip(const bytes_t& bs, const u32 num_threads) -> bytes_t
{
    if (!num_threads)
        throw std::runtime_error{"[pigzcpp] num_threads = 0"};

    if (bs.empty())
        return {};

    std::vector<bytes_t> results{num_threads};
    const auto segments = slice(bs.size(), num_threads);
    const auto last = num_threads - 1;

    fan_out(num_threads, [&](const auto i){
        compress_bytes(bs, segments.at(i), results.at(i), (i == last));
    });

    return flatten(results);
}

auto zip(const path_t& p, const u32 num_threads, const u64 chunk_size) -> bytes_t
{
    if (!num_threads)
        throw std::runtime_error{"[pigzcpp] num_threads = 0"};

    if (!chunk_size)
        throw std::runtime_error{"[pigzcpp] chunk_size = 0"};

    if (!std::filesystem::exists(p))
        throw std::runtime_error{"[pigzcpp] file does not exist"};

    const auto file_size = std::filesystem::file_size(p);
    if (file_size == 0)
        return {};

    std::vector<bytes_t> results{num_threads};
    const auto segments = slice(file_size, num_threads);
    const auto last = num_threads - 1;

    fan_out(num_threads, [&](const auto i){
        compress_file(p, segments.at(i), chunk_size, results.at(i), (i == last));
    });

    return flatten(results);
}

auto unzip(const bytes_t& in) -> bytes_t
{
    if (in.empty())
        return {};

    z_stream stream;

    stream.zalloc   = Z_NULL;
    stream.zfree    = Z_NULL;
    stream.opaque   = Z_NULL;
    stream.avail_in = in.size();
    stream.next_in  = reinterpret_cast<const Bytef*>(in.data());

    inflateInit(&stream);

    bytes_t res;
    bytes_t buf(defaults::chunk_size);

    while (true)
    {
        stream.avail_out = buf.size();
        stream.next_out  = reinterpret_cast<Bytef*>(buf.data());

        const auto ret = inflate(&stream, Z_NO_FLUSH);
        res.insert(res.end(), buf.begin(), buf.begin() + (buf.size() - stream.avail_out));

        if (ret == Z_STREAM_END || ret == Z_DATA_ERROR)
            break;
    }

    inflateEnd(&stream);
    res.shrink_to_fit();

    return res;
}

} // namespace pigzcpp

