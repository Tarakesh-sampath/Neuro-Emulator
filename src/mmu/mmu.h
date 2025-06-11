#ifndef MMU_H
#define MMU_H

#include <cstdint>
#include <array>
#include "cartridge.h"

using byte = uint8_t;
using WORD = uint16_t;

class MMU {
public:
    explicit MMU(Cartridge& cart);

    byte readByte(WORD addr) const;
    void writeByte(WORD addr, byte val);

    bool loadROM(const std::vector<byte>& romData);

private:
    Cartridge& cartridge;

    std::array<byte, 0x2000> vram{};      // 8KB VRAM
    std::array<byte, 0x2000> wram{};      // 8KB Work RAM
    std::array<byte, 0xA0> oam{};         // Sprite attribute memory
    std::array<byte, 0x80> ioRegs{};      // I/O registers
    std::array<byte, 0x7F> zram{};        // Zero page RAM

    byte readIO(WORD addr) const;
    void writeIO(WORD addr, byte val);
};

#endif // MMU_H