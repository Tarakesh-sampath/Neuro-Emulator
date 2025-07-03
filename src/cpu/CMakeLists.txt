# Define cpu library target
add_library(cpu
    CPU.cpp
    CPURegisters.h
    CPU.h
)

# Since CPU depends on Memory, link it
target_link_libraries(cpu PUBLIC memory)

# Include dirs for cpu lib users
target_include_directories(cpu PUBLIC
    ${PROJECT_SOURCE_DIR}/src/cpu
    ${PROJECT_SOURCE_DIR}/src/memory
)