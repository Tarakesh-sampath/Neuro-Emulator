#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "CPURegisters.h"
#include "memory/Memory.h"

// 16-bit Register pairs
enum class Reg16 {
    BC,
    DE,
    HL,
    SP,
    AF   // Added AF for stack operations PUSH/POP
};

// 8-bit Registers
enum class Reg8 {
    A,
    B,
    C,
    D,
    E,
    H,
    L
};

// Conditional flags for conditional jumps and calls
enum class Condition {
    NZ, // Non-zero (Z reset)
    Z,  // Zero (Z set)
    NC, // No carry (C reset)
    C   // Carry (C set)
};

class CPU {
public:
    CPU(Memory* mem, CPURegisters* regs);

    // Run one instruction step (fetch, decode, execute)
    void step();

    // Run N instructions
    void run(int steps);

    // Reset CPU
    void reset();

private:
    CPURegisters* registers;
    Memory* memory;

    uint8_t fetch();
    void decodeRun(uint8_t opcode);

    bool halted = false;
    bool ime = false;
    bool imePending = false;
    int cycles = 0;

    // 16-bit load
    void LD_rr_d16(Reg16 reg);
    void LD_a16_SP();
    void LD_HL_SP_r8();
    void LD_SP_HL();

    // 8-bit load
    void LD_pRR_r(Reg16 rr, Reg8 r);
    void LD_r_pRR(Reg8 r, Reg16 rr);
    void LD_r_r(Reg8 dst, Reg8 src);
    void LD_r_d8(Reg8 r);
    void LDH_pa8_a();
    void LDH_a_pa8();
    void LD_A_pC();
    void LD_pC_A();
    void LD_pa16_A();
    void LD_A_pa16();
    void LD_A_pHL_inc();
    void LD_A_pHL_dec();
    void LD_pHL_inc_A();
    void LD_pHL_dec_A();
    void LD_pHL_d8();

    // Miscellaneous
    void NOP();
    void DI();
    void EI();
    void PrefixCB();
    void STOP();
    void HALT();

    // Stack
    void POP_rr(Reg16 rr);
    void PUSH_rr(Reg16 rr);

    // JR Jumps
    void JR_Nr_r8(Condition cond);
    void JR_r8();

    // JP Jumps
    void JP_Nr_pa16(Condition cond);
    void JP_HL();
    void JP_a16();

    // CALL
    void CALL_Nr_a16(Condition cond);
    void CALL_a16();

    // RET
    void RET_Nr(Condition cond);
    void RET();
    void RETI();

    // 16-bit Arithmetic
    void ADD_HL_rr(Reg16 reg);
    void ADD_SP_r8();
    void INC_rr(Reg16 reg);
    void DEC_rr(Reg16 reg);

    // 8-bit Arithmetic
    void ADD_r(Reg8 dst);
    void ADD_pHL();
    void ADD_r8();
    void SUB_r(Reg8 src);
    void SUB_pHL();
    void SUB_r8();
    void ADC_r(Reg8 r);
    void ADC_pHL();
    void ADC_r8();
    void SBC_r(Reg8 r);
    void SBC_pHL();
    void INC_r(Reg8 r);
    void INC_pHL();
    void DEC_r(Reg8 r);
    void DEC_pHL();

    // Logical instructions
    void AND_r(Reg8 r);
    void AND_pHL();
    void OR_r(Reg8 r);
    void OR_pHL();
    void XOR_r(Reg8 r);
    void XOR_pHL();
    void CP_r(Reg8 r);
    void CP_pHL();
    void CP_d8();

    // Rotate/Shift (CB prefix)
    void RLC_r();
    void RRC_r();
    void RL_r();
    void RR_r();
    void SLA_r();
    void SRA_r();
    void SRL_r();
    void SWAP_r();
    void BIT_n_r();
    void RES_n_r();
    void SET_n_r();

    // Misc operations
    void DAA();
    void CPL();
    void CCF();
    void SCF();

    // Extra rotate operations (non-CB)
    void RLCA();
    void RLA();
    void RRCA();
    void RRA();

    // Immediate variants of arithmetic instructions
    void AND_d8();
    void OR_d8();
    void XOR_d8();
    void SUB_d8();
    void ADC_d8();
    void SBC_d8();

    // Restart instruction
    void RST(uint8_t addr);
};

#endif //CPU_H