#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <string>

class Memory {
public:
    Memory();

    // Load ROM from file into memory at 0x0000
    bool loadROM(const std::string& filename);

    // Read one byte from memory address
    uint8_t readByte(uint16_t address) const;

    // Write one byte to memory address
    void writeByte(uint16_t address, uint8_t value);

private:
    static constexpr size_t MEMORY_SIZE = 65536; // 64KB

    uint8_t data[MEMORY_SIZE];
};

#endif // MEMORY_H