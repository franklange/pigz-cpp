cmake_minimum_required(VERSION 3.10)
project(pigz-cpp)

find_package(Threads REQUIRED)

option(PIGZCPP_TESTS "Enable Tests" OFF)
option(PIGZCPP_EXAMPLES "Enable Examples" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build Type" FORCE)
endif()

add_library(${PROJECT_NAME} "")

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINKER_LANGUAGE          CXX
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

target_sources(${PROJECT_NAME} PRIVATE
    include/pigz-cpp/pigz.hpp

    src/pigz.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Threads::Threads
)

if (PIGZCPP_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (PIGZCPP_EXAMPLES)
    add_subdirectory(example)
endif()
