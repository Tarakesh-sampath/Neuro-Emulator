#ifndef GAMEBOY_CPU_H
#define GAMEBOY_CPU_H

#include <cstdint>
#include "memory.h"

// Register/flag masks for GB
constexpr uint8_t FLAG_Z = 1 << 7;
constexpr uint8_t FLAG_N = 1 << 6;
constexpr uint8_t FLAG_H = 1 << 5;
constexpr uint8_t FLAG_C = 1 << 4;

class CPU; // forward declaration for handler
typedef void (CPU::*OpHandler)(); //<-- move OUTSIDE the class so it is globally visible

class CPU {
public:
    CPU(Memory& mem);

    void step();
    void dumpState() const;

    uint16_t getPC() const { return pc; }
    uint8_t getA() const { return a; }
    friend void initCBOpTable(OpHandler* cbTable, CPU* cpu);

    OpHandler opTable[0x100];
    OpHandler cbOpTable[0x100];

    // (keep these public for easier testing/debug later if you wish)
    void op_UNIMPLEMENTED();
    

private:
    Memory& memory;
    uint8_t a, f, b, c, d, e, h, l;
    uint16_t sp, pc;
    uint8_t temp_mem;

    void op_NOP();
    void op_LD_BC_d16();
    void op_INC_BC();
    void op_INC_B();
    void op_DEC_B();
    void op_CB_PREFIX();

    void op_LD_r_r();
    void op_LD_r_d8();

    void initOpTable();

    uint8_t reg8_read(int idx) const;
    void reg8_write(int idx, uint8_t val);
    bool ime_enabled = false; // Interrupt Master Enable

    // ALU group
    void op_ADD_A_r();
    void op_ADC_A_r();
    void op_SUB_A_r();
    void op_SBC_A_r();
    void op_AND_A_r();
    void op_OR_A_r();
    void op_XOR_A_r();
    void op_CP_A_r();
    
    void op_ADD_A_d8();
    void op_ADC_A_d8();
    void op_SUB_A_d8();
    void op_SBC_A_d8();
    void op_AND_A_d8();
    void op_OR_A_d8();
    void op_XOR_A_d8();
    void op_CP_A_d8();

    // ALU helpers
    void alu_add(uint8_t value, bool carry);
    void alu_sub(uint8_t value, bool carry);
    void alu_and(uint8_t value);
    void alu_or(uint8_t value);
    void alu_xor(uint8_t value);
    void alu_cp(uint8_t value);
    
    
    //jump 8-bit
    void op_JR_r8();
    void op_JR_NZ_r8();  // 0x20
    void op_JR_Z_r8();   // 0x28
    void op_JR_NC_r8();  // 0x30
    void op_JR_C_r8();   // 0x38
    
    //jump 16-bit
    void op_JP_a16();
    
    // DI
    void op_DI();
    
    //Load 8-bit
    void op_LDH_a8_A();
    void op_LDH_A_a8();
    
    
    //Load-16bit
    void op_LD_SP_d16();
    void op_LD_a16_A();
    void op_LD_HL_d16();
    void op_LD_DE_d16();

    //Load HL
    void op_LD_A_indHL_inc();
    void op_LD_A_indHL_dec();
    void op_LD_indHL_inc_A();
    void op_LD_indHL_dec_A();
    void op_LD_indDE_A();
    
    
    
    //Rest
    void op_RST_18();
    void op_RST_38();
    
    //Call 16-bit
    void op_CALL_a16();
    
    //Return 16-bit
    void op_RET();
    void op_RET_NZ();
    void op_RET_Z();
    void op_RET_NC();
    void op_RET_C();
    
    //Push HL
    void op_PUSH_BC();
    void op_PUSH_DE();
    void op_PUSH_HL();
    void op_PUSH_AF();
    void pushStack(uint16_t value);
    
    //Pop Hl
    void op_POP_BC();
    void op_POP_DE();
    void op_POP_HL();
    void op_POP_AF();
    uint16_t popStack();
    
    //inc 16-bit
    void op_INC_HL();
    void op_INC_DE();
    void op_INC_E();
    void op_INC_D();

    
    //dec16-bit
    void op_DEC_BC();
    void op_DEC_DE();
    void op_DEC_E();
    void op_DEC_D();
        
};

#endif // GAMEBOY_CPU_H