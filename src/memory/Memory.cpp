#include "Memory.h"
#include <fstream>
#include <iostream>
#include <cstring>

Memory::Memory() {
    std::fill(std::begin(vram), std::end(vram), 0x00);
    std::fill(std::begin(eram), std::end(eram), 0x00);
    std::fill(std::begin(wram), std::end(wram), 0x00);
    std::fill(std::begin(oam), std::end(oam), 0x00);
    std::fill(std::begin(io), std::end(io), 0x00);
    std::fill(std::begin(hram), std::end(hram), 0x00);
    ie = 0;
}

bool Memory::loadROM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Memory::loadROM failed to open " << filename << std::endl;
        return false;
    }

    // Read file into vector
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size <= 0) {
        std::cerr << "Memory::loadROM empty file " << filename << std::endl;
        return false;
    }

    rom.resize(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(rom.data()), size)) {
        std::cerr << "Memory::loadROM error reading " << filename << std::endl;
        return false;
    }

    // Read cartridge type from header at 0x0147
    if (rom.size() > 0x147) {
        cart_type = rom[0x147];
        std::cerr << "Loaded ROM: size=" << rom.size() << " bytes, cart_type=0x" << std::hex << (int)cart_type << std::dec << std::endl;
    }

    return true;
}

uint8_t Memory::readByte(uint16_t address) const {
    // Implement basic memory map for reads
    if (address <= 0x3FFF) {
        // ROM bank 0
        if (rom.size() > address) return rom[address];
        return 0xFF;
    }

    if (address >= 0x4000 && address <= 0x7FFF) {
        // ROM bank 1 (no banking implemented yet, just map to offset)
        size_t idx = static_cast<size_t>(address);
        if (rom.size() > idx) return rom[idx];
        return 0xFF;
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        return vram[address - 0x8000];
    }

    if (address >= 0xA000 && address <= 0xBFFF) {
        return eram[address - 0xA000];
    }

    if (address >= 0xC000 && address <= 0xDFFF) {
        return wram[address - 0xC000];
    }

    if (address >= 0xFE00 && address <= 0xFE9F) {
        return oam[address - 0xFE00];
    }

    if (address >= 0xFF00 && address <= 0xFF7F) {
        return io[address - 0xFF00];
    }

    if (address >= 0xFF80 && address <= 0xFFFE) {
        return hram[address - 0xFF80];
    }

    if (address == 0xFFFF) return ie;

    // Echo region and unmapped areas: return 0xFF
    return 0xFF;
}

void Memory::writeByte(uint16_t address, uint8_t value) {
    // Implement writes to writable regions
    if (address >= 0x8000 && address <= 0x9FFF) {
        vram[address - 0x8000] = value;
        return;
    }

    if (address >= 0xA000 && address <= 0xBFFF) {
        eram[address - 0xA000] = value;
        return;
    }

    if (address >= 0xC000 && address <= 0xDFFF) {
        wram[address - 0xC000] = value;
        return;
    }

    if (address >= 0xFE00 && address <= 0xFE9F) {
        oam[address - 0xFE00] = value;
        return;
    }

    if (address >= 0xFF00 && address <= 0xFF7F) {
        // Special handling for timer registers:
        uint16_t idx = address - 0xFF00;
        // DIV (0xFF04) writing resets divider
        if (address == 0xFF04) {
            // Writing any value resets DIV to 0
            div_counter = 0;
            io[0x04] = 0;
            return;
        }

        // TIMA (0xFF05), TMA (0xFF06), TAC (0xFF07)
        if (address == 0xFF05) { io[0x05] = value; return; }
        if (address == 0xFF06) { io[0x06] = value; return; }
        if (address == 0xFF07) { io[0x07] = value; return; }

        io[idx] = value;
        return;
    }

    if (address >= 0xFF80 && address <= 0xFFFE) {
        hram[address - 0xFF80] = value;
        return;
    }

    if (address == 0xFFFF) { ie = value; return; }

    // ROM and other areas are ignored for writes for now
}

// Timer helpers
// Timer clock frequencies mapping (CPU clock is 4.194304 MHz; we count machine cycles)
static const int TAC_FREQ_TO_CYCLES[4] = {
    1024, // 4096 Hz -> 1024 cycles per increment of TIMA (at 1 MHz machine?)
    16,   // 262144 Hz -> 16 cycles
    64,   // 65536 Hz -> 64 cycles
    256   // 16384 Hz -> 256 cycles
};

void Memory::tickTimers(int cyclesElapsed) {
    // DIV is incremented at 16384 Hz i.e., every 256 machine cycles
    div_counter += cyclesElapsed;
    // Update DIV register (upper 8 bits of div_counter >> 8)
    // DIV register is the high byte of a 16-bit counter that increments at CPU clock/256
    io[0x04] = static_cast<uint8_t>((div_counter >> 8) & 0xFF);

    // TIMA handling
    uint8_t tac = io[0x07];
    bool timer_enabled = (tac & 0x04) != 0;
    uint8_t tac_freq = tac & 0x03;

    if (timer_enabled) {
        // Use mapping to cycles per TIMA increment
        int threshold = TAC_FREQ_TO_CYCLES[tac_freq];
        tima_counter += cyclesElapsed;
        while (tima_counter >= threshold) {
            tima_counter -= threshold;
            // increment TIMA
            uint8_t tima = io[0x05];
            tima++;
            if (tima == 0x00) {
                // overflow: reload from TMA and request timer interrupt (bit 2 of IF at 0xFF0F)
                io[0x05] = io[0x06];
                // set IF bit 2
                io[0x0F] |= (1 << 2);
            } else {
                io[0x05] = tima;
            }
        }
    }
}