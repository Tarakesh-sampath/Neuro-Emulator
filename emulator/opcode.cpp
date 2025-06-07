// /project/emulator/opcode.cpp
// Parses opcode.json and provides lookup for opcode info

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "opcode.h"

using json = nlohmann::json;

bool OpcodeTable::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open opcode file: " << path << std::endl;
        return false;
    }

    json j;
    file >> j;

    // Assuming your file structure has "unprefixed" and later maybe "cb"
    auto unprefixed = j["unprefixed"];

    for (auto it = unprefixed.begin(); it != unprefixed.end(); ++it) {
        std::string key = it.key(); // e.g. "0x00"
        uint8_t opcode = std::stoi(key, nullptr, 16);

        OpcodeInfo info;
        info.mnemonic = it.value()["mnemonic"].get<std::string>();
        info.length = it.value()["length"].get<uint8_t>();
        for (auto& c : it.value()["cycles"]) {
            info.cycles.push_back(c.get<int>());
        }
        for (auto& f : it.value()["flags"]) {
            info.flags.push_back(f.get<std::string>());
        }
        info.group = it.value()["group"].get<std::string>();

        opcodes[opcode] = info;
    }

    return true;
}

const OpcodeInfo* OpcodeTable::getOpcodeInfo(uint8_t opcode) const {
    auto it = opcodes.find(opcode);
    if (it != opcodes.end())
        return &it->second;
    return nullptr;
}