#include "memory.h"
#include <fstream>
#include <iostream>

Memory::Memory() : rom(ROM_SIZE, 0xFF), ram(RAM_SIZE, 0x00) {}

bool Memory::loadROM(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "[Memory] Failed to open ROM file: " << path << "\n";
        return false;
    }
    file.read(reinterpret_cast<char*>(rom.data()), ROM_SIZE);
    // If ROM < 32KB, leave rest as 0xFF; if larger, ignore banking for now
    if (file.gcount() < ROM_SIZE) {
        std::cout << "[Memory] ROM file smaller than 32KB, padding with 0xFF\n";
    }
    return true;
}

uint8_t Memory::readByte(uint16_t addr) const {
    if (addr >= ROM_START && addr <= ROM_END) {
        // Cartridge ROM (read-only)
        return rom[addr - ROM_START];
    }
    if (addr >= RAM_START && addr <= RAM_END) {
        // Internal RAM
        return ram[addr - RAM_START];
    }
    // TODO: Extend for VRAM, I/O, etc.
    return 0xFF; // Default unmapped value
}

void Memory::writeByte(uint16_t addr, uint8_t value) {
    if (addr >= RAM_START && addr <= RAM_END) {
        ram[addr - RAM_START] = value;
        return;
    }
    // Ignore ROM writes (or log attempt)
    // TODO: Extend for VRAM, I/O, etc.
}