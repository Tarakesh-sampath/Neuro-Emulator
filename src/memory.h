#ifndef GAMEBOY_MEMORY_H
#define GAMEBOY_MEMORY_H

#include <cstdint>
#include <vector>
#include <string>

// Memory map constants
constexpr uint16_t ROM_START = 0x0000;
constexpr uint16_t ROM_END = 0x7FFF;
constexpr uint16_t RAM_START = 0xC000;
constexpr uint16_t RAM_END = 0xDFFF;

class Memory {
public:
    static constexpr size_t ROM_SIZE = 0x8000;   // 32KB max (No banking, for now)
    static constexpr size_t RAM_SIZE = 0x2000;   // 8KB internal RAM

    Memory();

    // Load .gb ROM file into memory
    bool loadROM(const std::string& path);

    // General read/write
    uint8_t readByte(uint16_t addr) const;
    void writeByte(uint16_t addr, uint8_t value);

    // For testing/logging, raw pointers (careful with these)
    const uint8_t* getROMPtr() const { return rom.data(); }
    uint8_t* getRAMPtr() { return ram.data(); }

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;
};

#endif // GAMEBOY_MEMORY_H