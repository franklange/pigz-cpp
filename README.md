# pigz-cpp
C++20 parallel implementation of gzip compression using zlib.

This library's approach is heavily inspired by the original
[pigz](https://github.com/madler/pigz) implementation by
[Mark Adler](https://en.wikipedia.org/wiki/Mark_Adler).



### Goals
* made to be used as a library, not as a standalone command line tool
* only a fraction of the code size and complexity with a minimal performance hit

### Dependencies
* zlib

### Example
```c++
#include <pigz-cpp/pigz.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

auto main() -> int
{
    // compress arbitrary file
    const auto z = pigzcpp::zip("/files/inventory.xlsx");

    // compressed data is in zlib format and
    // can be manually decompressed, for example using zlib-flate
    std::ofstream of{"backup.zlib", std::ios::out | std::ios::trunc | std::ios::binary};
    of.write(reinterpret_cast<const char*>(z.data()), z.size());


    // decompress and write out, should be exact same file
    const auto uz = pigzcpp::unzip(z);

    std::ofstream of{"test/roundtrip.txt", std::ios::out | std::ios::trunc | std::ios::binary};
    of.write(reinterpret_cast<const char*>(uz.data()), uz.size());

    return 0;
}
```

### Build and run example
```bash
install zlib
git clone https://github.com/franklange/pigz-cpp.git
cd pigz-cpp
mkdir build
cd build
cmake -DPIGZCPP_EXAMPLES=ON ..
cmake --build . -j
./bin/pigz-cpp-example bin/files/alice.txt

```