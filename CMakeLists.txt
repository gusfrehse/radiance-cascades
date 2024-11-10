cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(RadianceCascades CXX)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(RadianceCascades WIN32 src/main.cpp)

target_link_libraries(RadianceCascades PRIVATE SDL3::SDL3)
