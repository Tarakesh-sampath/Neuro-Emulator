#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

class Memory {
public:
    Memory(const std::string& rom_path) {
        // Load ROM data
        std::ifstream romFile(rom_path, std::ios::binary | std::ios::ate);
        if (!romFile.is_open()) {
            std::cerr << "Failed to open ROM: " << rom_path << std::endl;
            exit(1);
        }

        std::streamsize size = romFile.tellg();
        romFile.seekg(0, std::ios::beg);

        rom.resize(size);
        if (!romFile.read(reinterpret_cast<char*>(rom.data()), size)) {
            std::cerr << "Failed to read ROM data." << std::endl;
            exit(1);
        }

        // Initialize other memory areas
        vram.resize(0x2000);  // 8KB VRAM
        wram.resize(0x2000);  // 8KB Work RAM
        eram.resize(0x2000);  // 8KB External RAM (simplified)
        oam.resize(0xA0);     // Sprite attribute table
        io_regs.resize(0x80); // I/O Registers
        hram.resize(0x7F);    // High RAM

        ie = 0x00;            // Interrupt Enable Register
    }

    uint8_t read_byte(uint16_t addr) {
        if (addr <= 0x7FFF) {
            // Reading from ROM
            if (addr < rom.size())
                return rom[addr];
            else
                return 0xFF; // Out of bounds, return 0xFF
        }
        else if (addr >= 0x8000 && addr <= 0x9FFF) {
            return vram[addr - 0x8000];
        }
        else if (addr >= 0xA000 && addr <= 0xBFFF) {
            return eram[addr - 0xA000];
        }
        else if (addr >= 0xC000 && addr <= 0xDFFF) {
            return wram[addr - 0xC000];
        }
        else if (addr >= 0xFE00 && addr <= 0xFE9F) {
            return oam[addr - 0xFE00];
        }
        else if (addr >= 0xFF00 && addr <= 0xFF7F) {
            return io_regs[addr - 0xFF00];
        }
        else if (addr >= 0xFF80 && addr <= 0xFFFE) {
            return hram[addr - 0xFF80];
        }
        else if (addr == 0xFFFF) {
            return ie;
        }
        else {
            return 0xFF;
        }
    }

    void write_byte(uint16_t addr, uint8_t value) {
        if (addr <= 0x7FFF) {
            // ROM is read-only, ignore writes here
        }
        else if (addr >= 0x8000 && addr <= 0x9FFF) {
            vram[addr - 0x8000] = value;
        }
        else if (addr >= 0xA000 && addr <= 0xBFFF) {
            eram[addr - 0xA000] = value;
        }
        else if (addr >= 0xC000 && addr <= 0xDFFF) {
            wram[addr - 0xC000] = value;
        }
        else if (addr >= 0xFE00 && addr <= 0xFE9F) {
            oam[addr - 0xFE00] = value;
        }
        else if (addr >= 0xFF00 && addr <= 0xFF7F) {
            io_regs[addr - 0xFF00] = value;
        }
        else if (addr >= 0xFF80 && addr <= 0xFFFE) {
            hram[addr - 0xFF80] = value;
        }
        else if (addr == 0xFFFF) {
            ie = value;
        }
        // Ignore other writes
    }

private:
    std::vector<uint8_t> rom;
    std::vector<uint8_t> vram;
    std::vector<uint8_t> wram;
    std::vector<uint8_t> eram;
    std::vector<uint8_t> oam;
    std::vector<uint8_t> io_regs;
    std::vector<uint8_t> hram;

    uint8_t ie;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: emulator <path_to_rom.gb>" << std::endl;
        return 1;
    }

    Memory memory(argv[1]);

    // Basic test: read first byte of ROM
    uint8_t firstByte = memory.read_byte(0x0000);
    std::cout << "First byte of ROM: 0x" << std::hex << (int)firstByte << std::endl;

    return 0;
}