#include "mmu.h"
#include <iostream>

MMU::MMU(Cartridge& cart) : cartridge(cart) { }

bool MMU::loadROM(const std::vector<byte>& romData) {
    return cartridge.loadROM(romData);
}

byte MMU::readByte(WORD addr) const {
    if (addr < 0x8000) {
        // ROM or MBC registers accessed via cartridge
        return cartridge.readROM(addr);
    } else if (addr >= 0x8000 && addr <= 0x9FFF) {
        // VRAM
        return vram[addr - 0x8000];
    } else if (addr >= 0xA000 && addr <= 0xBFFF) {
        // Cartridge external RAM
        return cartridge.readRAM(addr);
    } else if (addr >= 0xC000 && addr <= 0xDFFF) {
        // Work RAM
        return wram[addr - 0xC000];
    } else if (addr >= 0xE000 && addr <= 0xFDFF) {
        // Echo RAM (mirror of Work RAM)
        return wram[addr - 0xE000];
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        // OAM - sprite attribute table
        return oam[addr - 0xFE00];
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        // I/O registers
        return readIO(addr);
    } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        // High RAM (Zero Page)
        return zram[addr - 0xFF80];
    } else {
        std::cerr << "MMU: read from unknown address 0x" << std::hex << addr << "\n";
        return 0xFF;
    }
}

void MMU::writeByte(WORD addr, byte val) {
    if (addr < 0x8000) {
        // Writes in ROM area usually MBC commands (bank switching, ram enable)
        cartridge.writeROM(addr, val);
    } else if (addr >= 0x8000 && addr <= 0x9FFF) {
        // VRAM
        vram[addr - 0x8000] = val;
    } else if (addr >= 0xA000 && addr <= 0xBFFF) {
        // Cartridge RAM
        cartridge.writeRAM(addr, val);
    } else if (addr >= 0xC000 && addr <= 0xDFFF) {
        // Work RAM
        wram[addr - 0xC000] = val;
    } else if (addr >= 0xE000 && addr <= 0xFDFF) {
        // Echo RAM mirror
        wram[addr - 0xE000] = val;
    } else if (addr >= 0xFE00 && addr <= 0xFE9F) {
        // OAM
        oam[addr - 0xFE00] = val;
    } else if (addr >= 0xFF00 && addr <= 0xFF7F) {
        // I/O registers
        writeIO(addr, val);
    } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
        // Zero page RAM
        zram[addr - 0xFF80] = val;
    } else {
        std::cerr << "MMU: write to unknown address 0x" << std::hex << addr << " val: 0x" << (int)val << "\n";
    }
}

byte MMU::readIO(WORD addr) const {
    // Very simplified stub for I/O registers
    // Real implementation must handle hardware registers (timers, input, sound, LCD, etc)
    byte val = ioRegs[addr - 0xFF00];
    // You can add hardware-specific side effects here
    return val;
}

void MMU::writeIO(WORD addr, byte val) {
    // Simplified stub for I/O write
    ioRegs[addr - 0xFF00] = val;
    // Add hardware side effects here as needed
}