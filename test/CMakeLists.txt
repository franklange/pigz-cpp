project(pigzcpp_test)

find_package(GTest REQUIRED)
include(GoogleTest)

file(COPY files DESTINATION ${CMAKE_BINARY_DIR}/test)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE
    compressor_TEST.cpp
    loader_TEST.cpp
    pigzcpp_TEST.cpp
    utils_TEST.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pigz-cpp
    GTest::Main
)

gtest_discover_tests(${PROJECT_NAME})
