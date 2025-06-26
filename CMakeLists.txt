cmake_minimum_required(VERSION 3.15)
project(GameBoyEmulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories for components
add_subdirectory(src/cpu)
add_subdirectory(src/memory)

# Add executable target for main.cpp
add_executable(emulator main.cpp)

# Link CPU and Memory libraries to executable
target_link_libraries(emulator PRIVATE cpu memory)

# Include directories for executable
target_include_directories(cpu PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)