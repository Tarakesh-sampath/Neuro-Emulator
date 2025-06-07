// /project/emulator/opcode.h
// Opcode info and JSON parsing for LR35902 instruction set

#ifndef OPCODE_H
#define OPCODE_H

#include <string>
#include <vector>
#include <map>


struct OpcodeInfo {
    std::string mnemonic;
    uint8_t length;
    std::vector<int> cycles;
    std::vector<std::string> flags;
    std::string group;
};

class OpcodeTable {
public:
    bool loadFromFile(const std::string& path);
    const OpcodeInfo* getOpcodeInfo(uint8_t opcode) const;

private:
    std::map<uint8_t, OpcodeInfo> opcodes;
};

#endif // OPCODE_H