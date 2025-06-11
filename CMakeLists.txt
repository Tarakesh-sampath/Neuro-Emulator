cmake_minimum_required(VERSION 3.10)
project(GameBoyEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources as per finalized project structure
set(CPU_SOURCES
    src/cpu/cpu.cpp
    src/cpu/instr_table.cpp
    src/cpu/instr_func.cpp
    src/cpu/reg.cpp
)

set(CARTRIDGE_SOURCES
    src/cartridge/cartridge.cpp
)

set(MMU_SOURCES
    src/mmu/mmu.cpp
)

set(SRC_SOURCES
    src/gb.cpp
)

add_executable(GameBoyEmu
    ${CPU_SOURCES}
    ${CARTRIDGE_SOURCES}
    ${MMU_SOURCES}
    ${SRC_SOURCES}
)

target_include_directories(GameBoyEmu PRIVATE
    src/cpu
    src/cartridge
    src/mmu
)