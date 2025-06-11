#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <vector>
#include <cstdint>

using byte = uint8_t;
using WORD = uint16_t;

enum class MBCType {
    NONE,
    MBC1,
    // Other MBC types omitted for brevity
};

class Cartridge {
public:
    Cartridge();

    // Load ROM data
    bool loadROM(const std::vector<byte>& romData);

    // Read operations
    byte readROM(WORD addr) const;
    byte readRAM(WORD addr) const;

    // Write operations
    void writeROM(WORD addr, byte val);
    void writeRAM(WORD addr, byte val);

    MBCType getMBCType() const { return mbcType; }

private:
    std::vector<byte> rom;
    std::vector<byte> ram;

    MBCType mbcType = MBCType::NONE;

    // MBC1 state
    bool ramEnable = false;
    byte romBankLow5 = 1;     // lower 5 bits of ROM bank number
    byte bankMode = 0;        // 0 = ROM banking mode, 1 = RAM banking mode
    byte romBankHigh2 = 0;    // upper 2 bits of ROM bank number or RAM bank number depending on mode

    WORD getROMBank() const;
    WORD getRAMBank() const;
};

#endif // CARTRIDGE_H