//src/reg.cpp
#include "reg.h"

WORD Registers::getAF() const { return (A << 8) | F; }
WORD Registers::getBC() const { return (B << 8) | C; }
WORD Registers::getDE() const { return (D << 8) | E; }
WORD Registers::getHL() const { return (H << 8) | L; }

void Registers::setAF(WORD val) { A = val >> 8; F = val & 0xF0; }
void Registers::setBC(WORD val) { B = val >> 8; C = val & 0xFF; }
void Registers::setDE(WORD val) { D = val >> 8; E = val & 0xFF; }
void Registers::setHL(WORD val) { H = val >> 8; L = val & 0xFF; }

WORD Registers::getSP() const { return SP; }
WORD Registers::getPC() const { return PC; }

void Registers::setSP(WORD val) { SP = val; }
void Registers::setPC(WORD val) { PC = val; }

void Registers::setFlag(byte flag, bool on) {
    if (on) {
        F = (F | flag) & 0xF0;  // Force lower nibble zero
    } else {
        F = (F & ~flag) & 0xF0;
    }
}

bool Registers::getFlag(byte flag) const {
    return (F & flag) != 0;
}

void Registers::clearFlags() {
    F = 0;
}

CpuState Registers::snapshot() const {
    return CpuState{PC, SP, A, F, B, C, D, E, H, L};
}