cmake_minimum_required(VERSION 3.20.0)
project(knx-stack, VERSION 1.0  LANGUAGES C)


FILE(GLOB knxstack_sources lib/*.c platform/*.c)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wno-unknown-pragmas -g -O0 -Iinclude")

add_executable(knx-stack ${knxstack_sources})
target_link_libraries(knx-stack  "${LIBRARIES_FROM_REFERENCES}")
target_include_directories(knx-stack  PRIVATE include)
