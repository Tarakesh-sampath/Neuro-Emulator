#include "CPU.h"
#include <iostream>

// Constructor
CPU::CPU(Memory* mem, CPURegisters* regs) : memory(mem), registers(regs) {
    reset();
}

void CPU::reset() {
    // Initialize registers to known startup values
    registers->setAF(0x01B0);
    registers->setBC(0x0013);
    registers->setDE(0x00D8);
    registers->setHL(0x014D);
    registers->setSP(0xFFFE);
    registers->setPC(0x0100);  // Start of program in cartridge

    halted = false;
}

// Fetch next byte at PC
uint8_t CPU::fetch() {
    uint16_t pc = registers->getPC();
    uint8_t val = memory->readByte(pc);
    registers->setPC(pc + 1);
    return val;
}

// Step: fetch, decode, execute one instruction
void CPU::step() {
    if (halted)
        return;  // CPU halted, do nothing

    uint8_t opcode = fetch();
    decodeRun(opcode);
}

void CPU::run(int steps) {
    for (int i = 0; i < steps; i++) {
        step();
        if (halted)
            break;
    }
}

// Instruction decode and dispatch
void CPU::decodeRun(uint8_t opcode) {
    switch (opcode) {
        case 0x00:  // NOP
            NOP();
            break;
        case 0x01:  // LD BC,d16
            LD_rr_d16(Reg16::BC);
            break;
        case 0x02:
            LD_pRR_r(Reg16::BC,Reg8::A);
            break;

        default:
            std::cerr << "Unimplemented opcode 0x" << std::hex << (int)opcode << std::endl;
            break;
    }
}

/////////////////////////  Instruction  ////////////////////////////////

// 16-bit Load instruction stubs

void CPU::LD_rr_d16(Reg16 reg) {
    uint8_t low = fetch();
    uint8_t high = fetch();
    uint16_t d16 = (high << 8) | low;

    switch (reg) {
    case Reg16::BC:
        registers->setBC(d16);
        break;
    case Reg16::DE:
        registers->setDE(d16);
        break;
    case Reg16::HL:
        registers->setHL(d16);
        break;
    case Reg16::SP:
        registers->setSP(d16);
        break;
    default:
        // Handle illegal case or log error
        break;
    }

    // Optionally log instruction details for ML here
}

void CPU::LD_a16_SP() {
    // Stub for LD (a16),SP
    // TODO: Fetch 16-bit address and store Stack Pointer there
    // Log memory write and SP for ML
}

void CPU::LD_HL_SP_r8() {
    // Stub for LD HL,SP+r8
    // TODO: Fetch signed 8-bit offset and add to SP, load result into HL
    // Update flags per spec and log
}

void CPU::LD_rr_rr() {
    // Stub for LD rr,rr
    // TODO: Load 16-bit value from source register pair to destination register pair
    // No flags changed
    // Log register transfer
}

// 8-bit Load instruction stubs

void CPU::LD_pRR_r(Reg16 rr, Reg8 r) {
    uint16_t addr;
    switch (rr) {
        case Reg16::BC: addr = registers->getBC(); break;
        case Reg16::DE: addr = registers->getDE(); break;
        case Reg16::HL: addr = registers->getHL(); break;
        case Reg16::SP: addr = registers->getSP(); break;  // Usually SP not used here but included for completeness
        default: addr = 0; break;
    }

    uint8_t val;
    switch (r) {
        case Reg8::A: val = registers->getA(); break;
        case Reg8::B: val = registers->getB(); break;
        case Reg8::C: val = registers->getC(); break;
        case Reg8::D: val = registers->getD(); break;
        case Reg8::E: val = registers->getE(); break;
        case Reg8::H: val = registers->getH(); break;
        case Reg8::L: val = registers->getL(); break;
        default: val = 0; break;
    }

    memory->writeByte(addr, val);

    // Log memory write and register read for ML here
}

void CPU::LD_r_pRR() {
    // Stub for LD r, (rr)
    // TODO: Load 8-bit value from memory address in rr to register r
    // Log memory read and register write
}

void CPU::LD_r_r() {
    // Stub for LD r, r
    // TODO: Load 8-bit value from source register to destination register
    // Simple register to register copy, no flags affected
    // Log register reads and writes
}

void CPU::LDH_pa8_r() {
    // Stub for LDH (a8), r
    // TODO: Store 8-bit register r value into high RAM address (0xFF00 + a8)
    // Log memory write and immediate operand
}

void CPU::LDH_r_pa8() {
    // Stub for LDH r, (a8)
    // TODO: Load 8-bit value from high RAM (0xFF00 + a8) into register r
    // Log memory read and register write
}

void CPU::LD_A_pC() {
    // Stub for LD A, (C)
    // TODO: Load from I/O port address (0xFF00 + C) into register A
    // Log I/O read and register write
}

void CPU::LD_pC_A() {
    // Stub for LD (C), A
    // TODO: Store register A value into I/O port address (0xFF00 + C)
    // Log I/O write and register read
}

void CPU::LD_pa16_A() {
    // Stub for LD (a16), A
    // TODO: Store register A into memory at 16-bit immediate address
    // Log memory write
}

void CPU::LD_A_pa16() {
    // Stub for LD A, (a16)
    // TODO: Load into register A from memory at 16-bit immediate address
    // Log memory read
}

// Miscellaneous instruction stubs

void CPU::Nop() {
    // NOP - No operation
    // Log execution for ML
}

void CPU::Di() {
    // DI - Disable interrupts
    // TODO: Disable interrupt master flag
}

void CPU::Ei() {
    // EI - Enable interrupts (enable after next instruction)
    // TODO: Enable interrupt master flag
}

void CPU::PrefixCB() {
    // CB prefix instruction dispatch
    // Fetch next opcode and call DecodeRunCB()
}

void CPU::Stop() {
    // STOP 0 - Stops CPU until button press or similar
}

void CPU::Halt() {
    // HALT - Halt CPU until interrupt
    // Set halt flag state
}

// Stack

void CPU::Pop_rr() {
    // POP rr stub: Pop 16-bit word from stack (SP) into rr register pair
    // TODO:
    //  uint16_t sp = registers->getSP();
    //  uint8_t low = memory->readByte(sp);
    //  uint8_t high = memory->readByte(sp + 1);
    //  uint16_t value = (high << 8) | low;
    //  registers->set_rr(value); // Depending on decoded rr
    //  registers->setSP(sp + 2);

    // Log memory read and register write for ML
}

void CPU::Push_rr() {
    // PUSH rr stub: Push 16-bit value of rr register pair onto stack
    // TODO:
    //  uint16_t sp = registers->getSP() - 2;
    //  uint16_t value = registers->get_rr(); // Depending on decoded rr
    //  memory->writeByte(sp, value & 0xFF);
    //  memory->writeByte(sp + 1, value >> 8);
    //  registers->setSP(sp);

    // Log memory write and register read for ML
}

//JR Jumps

void CPU::JR_Nr_r8() {
    // Stub for JR Nr, r8 (conditional relative jump)
    // TODO:
    // Fetch signed 8-bit offset from PC
    // Check condition Nr (e.g. NZ, Z, NC, C)
    // If condition true, add offset to PC to jump
    // Log input flags, PC before/after, condition result for ML
}

void CPU::JR_r_a16() {
    // Stub for JR r, a16 (rare - jump to 16-bit address relative or absolute)
    // TODO:
    // Fetch 16-bit address and update PC accordingly
    // Log address changes
}

void CPU::JR_r8() {
    // Stub for JR r8 (unconditional relative jump)
    // TODO:
    // Fetch signed 8-bit offset and add to PC
    // Log jump details
}

// JP Jumps: Categories 25-28

void CPU::JP_Nr_pa16() {
    // JP Nr, (a16) - Conditional jump to 16-bit immediate address
    // TODO:
    //  Fetch 16-bit address (a16)
    //  Check condition Nr (e.g. NZ, Z, NC, C)
    //  If condition true, PC = a16 else PC++
    // Log PC changes, condition, address for ML instrumentation
}

void CPU::JP_r_pa16() {
    // JP r, (a16) - Jump to 16-bit address register indirect
    // TODO:
    //  Load address from 16-bit register r (BC, DE, HL, AF)
    //  Set PC = r
    // Log PC and register state
}

void CPU::JP_HL() {
    // JP (HL) - Jump to address contained in HL register
    // TODO:
    //  PC = HL
    // Log new PC state
}

void CPU::JP_a16() {
    // JP a16 - Unconditional jump to 16-bit immediate address
    // TODO:
    //  Fetch 16-bit immediate address 
    //  Set PC = a16
    // Log jump address and PC change
}

// CALL Instructions: Categories 29-31

void CPU::CALL_Nr_a16() {
    // CALL Nr, a16 - Conditional call to 16-bit immediate address
    // TODO:
    //  Fetch 16-bit address (a16)
    //  Check condition Nr (e.g., NZ, Z, NC, C)
    //  If condition true:
    //      Push PC+3 (next instruction) onto stack
    //      Set PC = a16
    //  Else:
    //      PC += 3 (skip call)
    // Log stack, PC changes, condition for ML instrumentation
}

void CPU::CALL_a16() {
    // CALL a16 - Unconditional call to 16-bit immediate address
    // TODO:
    //  Fetch 16-bit immediate address
    //  Push current PC+3 onto stack
    //  Set PC = a16
    // Log stack and PC changes
}

void CPU::CALL_conditional() {
    // CALL Nr, a16 (conditional call repeated variant)
    // This stub can be combined with CALL_Nr_a16 above
    // Possibly takes parameters to determine condition
    // For now, treat as placeholder
}

// RET Instructions: Categories 32-35

void CPU::RET_Nr() {
    // RET Nr - Conditional return from subroutine
    // TODO:
    //  Check condition Nr (NZ, Z, NC, C)
    //  If true:
    //      Pop PC from stack (read two bytes from SP, increment SP by 2)
    //      Set PC to popped address
    //  Else:
    //      PC += 1 (skip return)
    // Log stack read, PC update, and condition result
}

void CPU::RET_r() {
    // RET r - Return indirect via register (rare variant)
    // TODO: Depending on r, set PC to that register
    // Log PC update
}

void CPU::RET() {
    // RET - Unconditional return
    // TODO:
    //  Pop 16-bit PC from stack (SP), increment SP by 2
    //  Update PC with popped address
    // Log stack read and PC update
}

void CPU::RETI() {
    // RETI - Return and enable interrupts
    // TODO:
    //  Same as RET
    //  Enable interrupts (set IME flag)
    // Log stack, PC, and interrupt flag updates
}

// 16-bit Arithmetic Instructions: Categories 38-41

void CPU::ADD_HL_rr() {
    // Stub for ADD HL, rr
}

void CPU::ADD_SP_r8() {
    // Stub for ADD SP, r8
}

void CPU::INC_rr() {
    // Stub for INC rr
}

void CPU::DEC_rr() {
    // Stub for DEC rr
}

// 8-bit Arithmetic Instructions: Categories 42-53

void CPU::ADD_r_r() {
    // Stub for ADD r,r
}

void CPU::ADD_r_pHL() {
    // Stub for ADD r,(HL)
}

void CPU::SUB_r() {
    // Stub for SUB r
}

void CPU::SUB_pHL() {
    // Stub for SUB (HL)
}

void CPU::ADC_r() {
    // Stub for ADC r
}

void CPU::ADC_pHL() {
    // Stub for ADC (HL)
}

void CPU::SBC_r() {
    // Stub for SBC r
}

void CPU::SBC_pHL() {
    // Stub for SBC (HL)
}

void CPU::INC_r() {
    // Stub for INC r
}

// Logical Instructions: Categories 54-61

void CPU::AND_r() {
    // Stub for AND r
}

void CPU::AND_pHL() {
    // Stub for AND (HL)
}

void CPU::OR_r() {
    // Stub for OR r
}

void CPU::OR_pHL() {
    // Stub for OR (HL)
}

void CPU::XOR_r() {
    // Stub for XOR r
}

void CPU::XOR_pHL() {
    // Stub for XOR (HL)
}

void CPU::CP_r() {
    // Stub for CP r
}

void CPU::CP_pHL() {
    // Stub for CP (HL)
}

// Rotate/Shift (CB prefix) Instructions: Categories 62-69

void CPU::RLC_r() {
    // Stub for RLC r
}

void CPU::RRC_r() {
    // Stub for RRC r
}

void CPU::RL_r() {
    // Stub for RL r
}

void CPU::RR_r() {
    // Stub for RR r
}

void CPU::SLA_r() {
    // Stub for SLA r
}

void CPU::SRA_r() {
    // Stub for SRA r
}

void CPU::SRL_r() {
    // Stub for SRL r
}

void CPU::SWAP_r() {
    // Stub for SWAP r
}

void CPU::BIT_n_r() {
    // Stub for BIT n,r
}

void CPU::RES_n_r() {
    // Stub for RES n,r
}

void CPU::SET_n_r() {
    // Stub for SET n,r
}

// Miscellaneous Operations: Categories 70-73

void CPU::DAA() {
    // Stub for DAA
}

void CPU::CPL() {
    // Stub for CPL
}

void CPU::CCF() {
    // Stub for CCF
}

void CPU::SCF() {
    // Stub for SCF
}

void CPU::INC_pHL() {
    // Stub for INC (HL)
}

void CPU::DEC_r() {
    // Stub for DEC r
}

void CPU::DEC_pHL() {
    // Stub for DEC (HL)
}