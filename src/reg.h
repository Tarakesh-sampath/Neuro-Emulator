//src/reg.h
#ifndef REG_H
#define REG_H

#include <cstdint>

using byte = uint8_t;
using WORD = uint16_t;

constexpr byte ZF = 0x80; // Zero Flag
constexpr byte NF = 0x40; // Subtract Flag
constexpr byte HF = 0x20; // Half Carry Flag
constexpr byte CF = 0x10; // Carry Flag

struct CpuState {
    WORD PC;
    WORD SP;
    byte A, F, B, C, D, E, H, L;
};

class Registers {
public:
    Registers() = default;

    WORD getAF() const;
    WORD getBC() const;
    WORD getDE() const;
    WORD getHL() const;

    void setAF(WORD val);
    void setBC(WORD val);
    void setDE(WORD val);
    void setHL(WORD val);

    WORD getSP() const;
    WORD getPC() const;

    void setSP(WORD val);
    void setPC(WORD val);

    void setFlag(byte flag, bool on);
    bool getFlag(byte flag) const;
    void clearFlags();

    CpuState snapshot() const;

private:
    byte A = 0;
    byte F = 0; // lower nibble always 0
    byte B = 0;
    byte C = 0;
    byte D = 0;
    byte E = 0;
    byte H = 0;
    byte L = 0;

    WORD SP = 0;
    WORD PC = 0;
};

#endif // REG_H