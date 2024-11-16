cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(RadianceCascades CXX)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/glbinding EXCLUDE_FROM_ALL)

if(WIN32)
    set(CMAKE_PREFIX_PATH "./vendored/glbinding;${CMAKE_PREFIX_PATH}")
endif()

# find_package(glbinding REQUIRED)

add_executable(RadianceCascades
               src/main.cpp
               src/shaders.cpp
               src/shaders.hpp)

if(WIN32)
    add_custom_command(
        TARGET RadianceCascades POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:RadianceCascades>
        VERBATIM
    )
    
    add_custom_command(
        TARGET RadianceCascades POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:glbinding::glbinding> $<TARGET_FILE_DIR:RadianceCascades>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:glbinding::glbinding-aux> $<TARGET_FILE_DIR:RadianceCascades>
        VERBATIM
    )
endif()

target_link_libraries(RadianceCascades
                      PRIVATE
                      glbinding::glbinding
                      glbinding::glbinding-aux
                      SDL3::SDL3)
