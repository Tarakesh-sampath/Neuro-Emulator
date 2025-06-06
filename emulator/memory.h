// /project/emulator/memory.h
// Memory management and ROM loader for Game Boy emulator

#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <string>
#include <vector>

class Memory {
public:
    Memory(const std::string& rom_path);

    uint8_t read_byte(uint16_t addr);
    void write_byte(uint16_t addr, uint8_t value);

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> vram;
    std::vector<uint8_t> wram;
    std::vector<uint8_t> eram;
    std::vector<uint8_t> oam;
    std::vector<uint8_t> io_regs;
    std::vector<uint8_t> hram;

    uint8_t ie;
};

#endif // MEMORY_H