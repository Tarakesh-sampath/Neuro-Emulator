#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "cartridge/cartridge.h"
#include "mmu/mmu.h"
#include "cpu/cpu.h"

std::vector<uint8_t> loadROM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open ROM file: " << filename << "\n";
        return {};
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!file.read((char*)buffer.data(), size)) {
        std::cerr << "Failed to read ROM file: " << filename << "\n";
        return {};
    }
    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: emulator <romfile.gb> [--log]\n";
        return 1;
    }

    std::string romPath = argv[1];
    bool enableLogging = (argc > 2 && std::string(argv[2]) == "--log");

    auto romData = loadROM(romPath);
    if (romData.empty()) {
        return 1;
    }

    Cartridge cartridge;
    if (!cartridge.loadROM(romData)) {
        std::cerr << "Cartridge failed to load ROM.\n";
        return 1;
    }

    MMU mmu(cartridge);
    CPU cpu(mmu);

    cpu.reset();
    cpu.enableLogging(enableLogging);

    constexpr int maxSteps = 1000000;
    int step = 0;
    while (step < maxSteps) {
        try {
            cpu.step();
            // if your CPU exposes a halted flag, check here:
            // if (cpu.isHalted()) break;

            ++step;

            // Placeholder: add frame sync or timing once frame/timer integrated
            // std::this_thread::sleep_for(std::chrono::microseconds(frameDurationUs));
        } catch (const std::exception& e) {
            std::cerr << "Emulation error at step " << step << ": " << e.what() << "\n";
            break;
        }
    }

    std::cout << "Emulation finished after " << step << " steps.\n";
    if(enableLogging) {
        std::cout << "Instruction logging was ENABLED.\n";
    }

    return 0;
}