# Define memory library target
add_library(memory
    Memory.cpp
    Memory.h
)

# Include dirs for memory lib users
target_include_directories(memory PUBLIC
    ${PROJECT_SOURCE_DIR}/src/memory
)