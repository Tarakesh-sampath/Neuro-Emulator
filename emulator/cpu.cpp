// /project/emulator/cpu.cpp
#include <iostream>
#include "memory.h"
#include "opcode.h" // Header for OpcodeTable & OpcodeInfo

class CPU {
public:
    CPU(Memory& mem, const OpcodeTable& table) : mem(mem), opcodes(table), PC(0x0100) {
        A = F = B = C = D = E = H = L = 0;
        SP = 0xFFFE;
    }

    void Step() {
        uint8_t opcode = mem.read_byte(PC);
        const OpcodeInfo* info = opcodes.getOpcodeInfo(opcode);
        if (!info) {
            std::cerr << "Unknown opcode: 0x" << std::hex << (int)opcode << " at PC=0x" << PC << std::endl;
            PC += 1;
            return;
        }

        std::cout << "Opcode 0x" << std::hex << (int)opcode 
                  << ": " << info->mnemonic << " (Length " << std::dec << (int)info->length << ")" << std::endl;

        // For this example, just advance PC based on instruction length
        PC += info->length;

        // TODO: Call execution functions based on info->mnemonic

    }
private:
    Memory& mem;
    const OpcodeTable& opcodes;
    uint16_t PC, SP;
    uint8_t A, F, B, C, D, E, H, L;
};