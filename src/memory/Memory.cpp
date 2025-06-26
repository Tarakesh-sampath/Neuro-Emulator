#include "Memory.h"
#include <fstream>
#include <iostream>
#include <cstring>

Memory::Memory() {
    // Initialize all memory to 0xFF by default
    memset(data, 0xFF, MEMORY_SIZE);
}

bool Memory::loadROM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Memory::loadROM failed to open " << filename << std::endl;
        return false;
    }

    // Read ROM into beginning of memory
    file.read(reinterpret_cast<char*>(data), MEMORY_SIZE);

    if (file.bad()) {
        std::cerr << "Memory::loadROM error reading " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

uint8_t Memory::readByte(uint16_t address) const {
    // Simple bounds check could be added if desired
    return data[address];
}

void Memory::writeByte(uint16_t address, uint8_t value) {
    // For now allow write everywhere — memory mapping and cartridge restrictions will come later
    data[address] = value;
}