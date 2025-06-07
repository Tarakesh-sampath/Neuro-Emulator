// /project/emulator/memory.h
// Memory management and ROM loader for Game Boy emulator

#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <string>
#include <vector>
#include "joypad.h"

class Memory {
public:
    Memory(const std::string& rom_path);

    uint8_t read_byte(uint16_t addr);
    void write_byte(uint16_t addr, uint8_t value);
    Joypad joypad;

    uint8_t ie;

    std::vector<uint8_t> rom;
    std::vector<uint8_t> vram;
    std::vector<uint8_t> wram;
    std::vector<uint8_t> eram;
    std::vector<uint8_t> oam;
    std::vector<uint8_t> io_regs;
    std::vector<uint8_t> hram;
    
private:

    uint8_t currentRomBank = 1;
    uint8_t currentRamBank = 0;
    bool ramEnabled = false;
    uint8_t bankingMode = 0;  // 0 = ROM banking mode, 1 = RAM banking mode
};

#endif // MEMORY_H