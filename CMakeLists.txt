cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(RadianceCascades CXX)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(RadianceCascades src/main.cpp)

add_custom_command(
    TARGET RadianceCascades POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:RadianceCascades>
    VERBATIM
)

target_link_libraries(RadianceCascades PRIVATE SDL3::SDL3)
