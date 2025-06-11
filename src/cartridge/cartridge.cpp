#include "cartridge.h"
#include <iostream>

Cartridge::Cartridge() = default;

bool Cartridge::loadROM(const std::vector<byte>& romData) {
    if (romData.size() < 0x8000) {
        std::cerr << "ROM too small!\n";
        return false;
    }
    rom = romData;

    // Parse cartridge type byte at 0x147 for MBC detection
    byte cartType = rom[0x147];
    switch (cartType) {
        case 0x00: mbcType = MBCType::NONE; break;
        case 0x01: case 0x02: case 0x03: mbcType = MBCType::MBC1; break;
        default:
            std::cerr << "Unsupported cartridge type: 0x" << std::hex << (int)cartType << "\n";
            return false;
    }

    // Allocate RAM if cartridge supports it (based on header 0x149)
    byte ramSize = rom[0x149];
    switch (ramSize) {
        case 0x00: ram.clear(); break;
        case 0x02: ram.resize(8 * 1024); break;   // 8KB RAM
        case 0x03: ram.resize(32 * 1024); break;  // 32KB RAM
        default: ram.clear(); break;               // Other sizes unsupported for now
    }

    return true;
}

byte Cartridge::readROM(WORD addr) const {
    if (addr < 0x4000) {
        // Bank 0 fixed
        return rom[addr];
    } else if (addr < 0x8000) {
        // Switchable ROM bank
        WORD bank = getROMBank();
        WORD offset = (bank * 0x4000) + (addr - 0x4000);
        if (offset < rom.size())
            return rom[offset];
        else
            return 0xFF; // Open bus fallback
    }
    return 0xFF;
}

byte Cartridge::readRAM(WORD addr) const {
    if (!ramEnable || ram.empty()) return 0xFF;
    WORD bank = getRAMBank();
    WORD offset = bank * 0x2000 + (addr - 0xA000);
    if (offset < ram.size())
        return ram[offset];
    return 0xFF;
}

void Cartridge::writeROM(WORD addr, byte val) {
    if (mbcType != MBCType::MBC1) return;

    if (addr >= 0x0000 && addr <= 0x1FFF) {
        // RAM enable/disable
        ramEnable = ((val & 0x0F) == 0x0A);
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        // ROM bank number low 5 bits
        val &= 0x1F;
        if (val == 0) val = 1; // Bank 0 prohibited
        romBankLow5 = val;
    } else if (addr >= 0x4000 && addr <= 0x5FFF) {
        // RAM bank number or upper ROM bank bits
        val &= 0x03;
        romBankHigh2 = val;
    } else if (addr >= 0x6000 && addr <= 0x7FFF) {
        // Banking mode select
        bankMode = val & 0x01;
    }
}

void Cartridge::writeRAM(WORD addr, byte val) {
    if (!ramEnable || ram.empty()) return;
    WORD bank = getRAMBank();
    WORD offset = bank * 0x2000 + (addr - 0xA000);
    if (offset < ram.size())
        ram[offset] = val;
}

WORD Cartridge::getROMBank() const {
    if (bankMode == 0) {
        // ROM banking mode
        return (romBankHigh2 << 5) | romBankLow5;
    } else {
        // RAM banking mode: high bits ignored in ROM bank
        return romBankLow5;
    }
}

WORD Cartridge::getRAMBank() const {
    if (bankMode == 0) {
        return 0;
    } else {
        return romBankHigh2;
    }
}