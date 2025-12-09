#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <string>
#include <vector>

class Memory {
public:
    Memory();

    // Load ROM from file into internal ROM buffer
    bool loadROM(const std::string& filename);

    // Read one byte from memory address (implements GameBoy memory map)
    uint8_t readByte(uint16_t address) const;

    // Write one byte to memory address (implements writes to RAM/IO)
    void writeByte(uint16_t address, uint8_t value);

    // Cartridge info
    auto cartridgeType() const -> uint8_t { return cart_type; }
    auto isROMOnly() const -> bool { return cart_type == 0x00; }

private:
    std::vector<uint8_t> rom;     // Cartridge ROM (variable size)

    // Memory regions
    uint8_t vram[0x2000]; // 0x8000 - 0x9FFF
    uint8_t eram[0x2000];  // 0xA000 - 0xBFFF (external RAM)
    uint8_t wram[0x2000];  // 0xC000 - 0xDFFF
    uint8_t oam[0xA0];     // 0xFE00 - 0xFE9F
    uint8_t io[0x80];      // 0xFF00 - 0xFF7F
    uint8_t hram[0x7F];    // 0xFF80 - 0xFFFE
    uint8_t ie;            // 0xFFFF
    // Timer internal state
    uint16_t div_counter = 0; // internal 16-bit divider
    uint16_t tima_counter = 0; // counts clock cycles for TIMA increments
    
public:
    // Advance timers by number of machine cycles (clock cycles)
    void tickTimers(int cycles);

    uint8_t cart_type = 0;
};

#endif // MEMORY_H