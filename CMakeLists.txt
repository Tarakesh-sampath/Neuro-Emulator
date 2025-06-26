cmake_minimum_required(VERSION 3.10)
project(GameBoyEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/cpu)
add_subdirectory(src/memory)

add_executable(gb_emulator main.cpp)

target_link_libraries(gb_emulator PRIVATE gb_cpu gb_memory)