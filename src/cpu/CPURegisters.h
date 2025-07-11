#ifndef CPUREGISTERS_H
#define CPUREGISTERS_H

class CPURegisters {
private:
    uint8_t A, F;  // Accumulator and Flag register (F keep masked)
    uint8_t B, C;
    uint8_t D, E;
    uint8_t H, L;
    uint16_t SP;
    uint16_t PC;

    // Mask for lower nibble of F - always zero
    static constexpr uint8_t FLAG_MASK = 0xF0;

public:
    // 16-bit register pairs used by instructions

    CPURegisters()
        : A(0), F(0), B(0), C(0), D(0), E(0), H(0), L(0), SP(0), PC(0)
    {}

    // 8-bit access
    uint8_t& getA() { return A; }
    void setA(uint8_t val) { A = val; }

    uint8_t& getF() { return F; }
    void setF(uint8_t val) { F = val & FLAG_MASK; }  // Mask flags bits lower nibble

    uint8_t& getB() { return B; }
    void setB(uint8_t val) { B = val; }

    uint8_t& getC() { return C; }
    void setC(uint8_t val) { C = val; }

    uint8_t& getD() { return D; }
    void setD(uint8_t val) { D = val; }

    uint8_t& getE() { return E; }
    void setE(uint8_t val) { E = val; }

    uint8_t& getH() { return H; }
    void setH(uint8_t val) { H = val; }

    uint8_t& getL() { return L; }
    void setL(uint8_t val) { L = val; }

    uint16_t& getSP() { return SP; }
    void setSP(uint16_t val) { SP = val; }

    uint16_t& getPC() { return PC; }
    void setPC(uint16_t val) { PC = val; }

    // 16-bit combined registers accessor
    uint16_t getAF() const { return (A << 8) | F; }
    void setAF(uint16_t val) {
        A = (val >> 8) & 0xFF;
        setF(val & 0xFF);
    }

    uint16_t getBC() const { return (B << 8) | C; }
    void setBC(uint16_t val) {
        B = (val >> 8) & 0xFF;
        C = val & 0xFF;
    }

    uint16_t getDE() const { return (D << 8) | E; }
    void setDE(uint16_t val) {
        D = (val >> 8) & 0xFF;
        E = val & 0xFF;
    }

    uint16_t getHL() const { return (H << 8) | L; }
    void setHL(uint16_t val) {
        H = (val >> 8) & 0xFF;
        L = val & 0xFF;
    }

    // Flag helpers:
    bool getFlagZ() const { return (F & 0x80) != 0; }
    void setFlagZ(bool val) { F = val ? (F | 0x80) : (F & ~0x80); }

    bool getFlagN() const { return (F & 0x40) != 0; }
    void setFlagN(bool val) { F = val ? (F | 0x40) : (F & ~0x40); }

    bool getFlagH() const { return (F & 0x20) != 0; }
    void setFlagH(bool val) { F = val ? (F | 0x20) : (F & ~0x20); }

    bool getFlagC() const { return (F & 0x10) != 0; }
    void setFlagC(bool val) { F = val ? (F | 0x10) : (F & ~0x10); }
};

#endif // CPUREGISTERS_H