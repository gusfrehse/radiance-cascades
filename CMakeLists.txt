cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(RadianceCascades C)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(RadianceCascades WIN32 main.c)

target_link_libraries(RadianceCascades PRIVATE SDL3::SDL3)
