#include <pigz-cpp/pigz.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

auto main(int, char** argv) -> int
{
    std::cout << "original: " << std::filesystem::file_size(argv[1]) << std::endl;

    const auto z = pigzcpp::zip(argv[1]);
    std::cout << "zipped:   " << z.size() << std::endl;

    const auto uz = pigzcpp::unzip(z);
    std::cout << "unzipped: " << uz.size() << std::endl;

    return 0;
}
