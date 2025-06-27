#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "CPURegisters.h"
#include "memory/Memory.h"


enum class Reg16 {
    BC,
    DE,
    HL,
    SP
};

// 8-bit registers
enum class Reg8 {
    A,
    B,
    C,
    D,
    E,
    H,
    L
};

class CPU {
public:
    
    CPU(Memory* mem, CPURegisters* regs);


    // Perform one CPU instruction step (fetch, decode, execute)
    void step();

    // Run CPU for given number of steps (instructions)
    void run(int steps);

    // Reset CPU state to initial power-on condition
    void reset();

private:
    CPURegisters* registers;
    Memory* memory;

    // Fetch next opcode from memory at PC
    uint8_t fetch();

    // Decode and execute given opcode
    void decodeRun(uint8_t opcode);

    bool halted = false;  // HALT state flag
    
    // 16-bit load
    void LD_rr_d16();        // LD rr,d16
    void LD_a16_SP();        // LD (a16),SP
    void LD_HL_SP_r8();      // LD HL,SP+r8
    void LD_rr_rr();         // LD rr,rr

    // 8-bit load
    void LD_pRR_r();         // LD (rr),r
    void LD_r_pRR();         // LD r,(rr)
    void LD_r_r();           // LD r,r
    void LDH_pa8_r();        // LDH (a8),r
    void LDH_r_pa8();        // LDH r,(a8)
    void LD_A_pC();          // LD A,(C)
    void LD_pC_A();          // LD (C),A
    void LD_pa16_A();        // LD (a16),A
    void LD_A_pa16();        // LD A,(a16)

    // Miscellaneous
    void NOP();
    void DI();
    void EI();
    void PrefixCB();
    void STOP();
    void HALT();

    // Stack
    void POP_rr();
    void PUSH_rr();

    // JR Jumps
    void JR_Nr_r8();
    void JR_r_a16();
    void JR_r8();

    // JP Jumps
    void JP_Nr_pa16();
    void JP_r_pa16();
    void JP_HL();
    void JP_a16();

    // CALL
    void CALL_Nr_a16();
    void CALL_a16();
    void CALL_conditional();

    // RET
    void RET_Nr();
    void RET_r();
    void RET();
    void RETI();

    // 16-bit Arithmetic
    void ADD_HL_rr();
    void ADD_SP_r8();
    void INC_rr();
    void DEC_rr();

    // 8-bit Arithmetic
    void ADD_r_r();
    void ADD_r_pHL();
    void SUB_r();
    void SUB_pHL();
    void ADC_r();
    void ADC_pHL();
    void SBC_r();
    void SBC_pHL();
    void INC_r();
    void INC_pHL();
    void DEC_r();
    void DEC_pHL();

    // Logical instructions
    void AND_r();
    void AND_pHL();
    void OR_r();
    void OR_pHL();
    void XOR_r();
    void XOR_pHL();
    void CP_r();
    void CP_pHL();

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

};

#endif //CPU_H