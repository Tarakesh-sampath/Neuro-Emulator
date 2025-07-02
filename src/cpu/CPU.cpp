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
            
        case 0x02:  // LD (BC), A
            LD_pRR_r(Reg16::BC, Reg8::A);
            break;
            
        case 0x08:  // LD (a16), SP
            LD_a16_SP();
            break;
        
        case 0x09:  // ADD HL, BC
            ADD_HL_rr(Reg16::BC);
            break;
                    
        case 0x0A: // LD A,(BC)
            LD_r_pRR(Reg8::A, Reg16::BC);
            break;
        
        case 0x10:  // STOP 0
            STOP();
            break;
            
        case 0x11: // LD DE,d16
            LD_rr_d16(Reg16::DE);
            break;
            
        case 0x12:  // LD (DE), A
            LD_pRR_r(Reg16::DE, Reg8::A);
            break;
        
        case 0x18:  // JR r8 (unconditional relative jump)
            JR_r8();
            break;
          
        case 0x19:  // ADD HL, DE
            ADD_HL_rr(Reg16::DE);
            break;
        
        case 0x1A: // LD A,(DE)
            LD_r_pRR(Reg8::A, Reg16::DE);
            break;
        
        case 0x20:  // JR NZ,r8
            JR_Nr_r8(Condition::NZ);
            break;
            
        case 0x21: // LD HL,d16
            LD_rr_d16(Reg16::HL);
            break;
            
        case 0x29:  // ADD HL, HL
            ADD_HL_rr(Reg16::HL);
            break;
    
        case 0x28:  // JR Z,r8
            JR_Nr_r8(Condition::Z);
            break;
            
        case 0x30:  // JR NC,r8
            JR_Nr_r8(Condition::NC);
            break;
            
        case 0x31: // LD SP,d16
            LD_rr_d16(Reg16::SP);
            break;
            
        case 0x38:  // JR C,r8
            JR_Nr_r8(Condition::C);
            break;
            
        case 0x39:  // ADD HL, SP
            ADD_HL_rr(Reg16::SP);
            break;
    
        case 0x40: // LD B B 
            LD_r_r(Reg8::B, Reg8::B);
            break;
            
        case 0x41: // LD B C
            LD_r_r(Reg8::B, Reg8::C);
            break;
            
        case 0x42: // LD B D
            LD_r_r(Reg8::B, Reg8::D); 
            break;
        
        case 0x43: // LD B E
            LD_r_r(Reg8::B, Reg8::E); 
            break;
            
        case 0x44: // LD B H
            LD_r_r(Reg8::B, Reg8::H); 
            break;
            
        case 0x45: // LD B L
            LD_r_r(Reg8::B, Reg8::L); 
            break;
            
        case 0x47: // LD B A
            LD_r_r(Reg8::B, Reg8::A); 
            break;
            
        case 0x48: // LD C B
            LD_r_r(Reg8::C, Reg8::B); 
            break;
            
        case 0x49: // LD C C
            LD_r_r(Reg8::C, Reg8::C); 
            break;
            
        case 0x4A: // LD C D
            LD_r_r(Reg8::C, Reg8::D); 
            break;
            
        case 0x4B: // LD C E
            LD_r_r(Reg8::C, Reg8::E); 
            break;
            
        case 0x4C: // LD C H
            LD_r_r(Reg8::C, Reg8::H); 
            break;
            
        case 0x4D: // LD C L
            LD_r_r(Reg8::C, Reg8::L); 
            break;
            
        case 0x4F: // LD C A
            LD_r_r(Reg8::C, Reg8::A); 
            break;
            
        case 0x50: // LD D B
            LD_r_r(Reg8::D, Reg8::B); 
            break;
            
        case 0x51: // LD D C
            LD_r_r(Reg8::D, Reg8::C); 
            break;
            
        case 0x52: // LD D D
            LD_r_r(Reg8::D, Reg8::D); 
            break;
            
        case 0x53: // LD D E
            LD_r_r(Reg8::D, Reg8::E); 
            break;
            
        case 0x54: // LD D H
            LD_r_r(Reg8::D, Reg8::H); 
            break;
            
        case 0x55: // LD D L
            LD_r_r(Reg8::D, Reg8::L); 
            break;
            
        case 0x57: // LD D A
            LD_r_r(Reg8::D, Reg8::A); 
            break;
            
        case 0x58: // LD E B
            LD_r_r(Reg8::E, Reg8::B); 
            break;
            
        case 0x59: // LD E C
            LD_r_r(Reg8::E, Reg8::C); 
            break;
            
        case 0x5A: // LD E D
            LD_r_r(Reg8::E, Reg8::D); 
            break;
            
        case 0x5B: // LD E E
            LD_r_r(Reg8::E, Reg8::E); 
            break;
            
        case 0x5C: // LD E H
            LD_r_r(Reg8::E, Reg8::B); 
            break;
            
        case 0x5D: // LD E L
            LD_r_r(Reg8::E, Reg8::L); 
            break;
            
        case 0x5F: // LD E A
            LD_r_r(Reg8::E, Reg8::A); 
            break;
            
        case 0x60: // LD H B
            LD_r_r(Reg8::H, Reg8::B); 
            break;
            
        case 0x61: // LD H C
            LD_r_r(Reg8::H, Reg8::C); 
            break;
            
        case 0x62: // LD H D
            LD_r_r(Reg8::H, Reg8::D); 
            break;
            
        case 0x63: // LD H E
            LD_r_r(Reg8::H, Reg8::E); 
            break;
            
        case 0x64: // LD H H
            LD_r_r(Reg8::H, Reg8::H); 
            break;
            
        case 0x65: // LD H L
            LD_r_r(Reg8::H, Reg8::L); 
            break;
            
        case 0x67: // LD H A
            LD_r_r(Reg8::H, Reg8::A); 
            break;
            
        case 0x68: // LD L B
            LD_r_r(Reg8::L, Reg8::B); 
            break;
            
        case 0x69: // LD L C
            LD_r_r(Reg8::L, Reg8::C); 
            break;
            
        case 0x6A: // LD L D
            LD_r_r(Reg8::L, Reg8::D); 
            break;
            
        case 0x6B: // LD L E
            LD_r_r(Reg8::L, Reg8::E); 
            break;
            
        case 0x6C: // LD L H
            LD_r_r(Reg8::L, Reg8::H); 
            break;
            
        case 0x6D: // LD L L
            LD_r_r(Reg8::L, Reg8::L); 
            break;
            
        case 0x6F: // LD L A
            LD_r_r(Reg8::L, Reg8::A); 
            break;
                
        case 0x77:  // LD (HL), A 
            LD_pRR_r(Reg16::HL, Reg8::A);
            break;
        
        case 0x76:  // HALT
            HALT();
            break;
            
        case 0x78: // LD A B
            LD_r_r(Reg8::A, Reg8::B); 
            break;
            
        case 0x79: // LD A C
            LD_r_r(Reg8::A, Reg8::C); 
            break;
            
        case 0x7A: // LD A D
            LD_r_r(Reg8::A, Reg8::D); 
            break;
            
        case 0x7B: // LD A E
            LD_r_r(Reg8::A, Reg8::E); 
            break;
            
        case 0x7C: // LD A H
            LD_r_r(Reg8::A, Reg8::H); 
            break;
            
        case 0x7D: // LD A L
            LD_r_r(Reg8::A, Reg8::D); 
            break;
            
        case 0x7E: // LD A,(HL)
            LD_r_pRR(Reg8::A, Reg16::HL);
            break;
            
        case 0x7F: // LD A A
            LD_r_r(Reg8::A, Reg8::A); 
            break;
            
        case 0xC0:  // RET NZ
            RET_Nr(Condition::NZ);
            break;
            
        case 0xC1:  // POP BC
            POP_rr(Reg16::BC);
            break;
            
        case 0xC2:  // JP NZ, a16
            JP_Nr_pa16(Condition::NZ);
            break;
        
        case 0xC3:  // JP a16
            JP_a16();
            break;
        
        case 0xC4:  // CALL NZ, a16
            CALL_Nr_a16(Condition::NZ);
            break;
            
        case 0xC5:  // PUSH BC
            PUSH_rr(Reg16::BC);
            break;
        
        case 0xC8:  // RET Z
            RET_Nr(Condition::Z);
            break;
            
        case 0xC9:  // RET (unconditional)
            RET();
            break;
        
        case 0xCA:  // JP Z, a16
            JP_Nr_pa16(Condition::Z);
            break;
            
        case 0xCB:  // Prefix CB instructions
            PrefixCB();
            break;
        
        case 0xCC:  // CALL Z, a16
            CALL_Nr_a16(Condition::Z);
            break;
        
        case 0xCD:  // CALL a16
            CALL_a16();
            break;
    
        case 0xD0:  // RET NC
            RET_Nr(Condition::NC);
            break;
            
        case 0xD1:  // POP DE
            POP_rr(Reg16::DE);
            break;
            
        case 0xD2:  // JP NC, a16
            JP_Nr_pa16(Condition::NC);
            break;
            
        case 0xD4:  // CALL NC, a16
            CALL_Nr_a16(Condition::NC);
            break;
    
        case 0xD5:  // PUSH DE
            PUSH_rr(Reg16::DE);
            break;
            
        case 0xD8:  // RET C
            RET_Nr(Condition::C);
            break;
            
        case 0xD9:  // RETI
            RETI();
            break;
            
        case 0xDA:  // JP C, a16
            JP_Nr_pa16(Condition::C);
            break;
        
        case 0xDC:  // CALL C, a16
            CALL_Nr_a16(Condition::C);
            break;
            
        case 0xE0:  // LDH (a8), A
            LDH_pa8_a();
            break;
        
        case 0xE1:  // POP HL
            POP_rr(Reg16::HL);
            break;    
                
        case 0xE2:  // LD (C), A
            LD_pC_A();
            break;
            
        case 0xE5:  // PUSH HL
            PUSH_rr(Reg16::HL);
            break;
            
        case 0xE8:  // ADD SP, r8 
            ADD_SP_r8();
            break;
            
        case 0xE9:  // JP (HL)
            JP_HL();
            break;
            
        case 0xEA:  // LD (a16), A
            LD_pa16_A();
            break;
            
        case 0xF0:  // LDH A, (a8)
            LDH_a_pa8();
            break;
        
        case 0xF1:  // POP AF
            POP_rr(Reg16::SP);  // For AF, you may want a specific enum or handle differently since AF not in Reg16 enum; alternatively add AF in Reg16
            break;
            
        case 0xF2:  // LD A,(C)
            LD_A_pC();
            break;
        
        case 0xF3:  // DI
            DI();
            break;

        case 0xF5:  // PUSH AF
            PUSH_rr(Reg16::AF);
            break;
            
        case 0xF8:  // LD HL, SP+r8
            LD_HL_SP_r8();
            break;
        
        case 0xF9:  // LD SP, HL
            LD_SP_HL();
            break;

        case 0xFA:  // LD A,(a16)
            LD_A_pa16();
            break;
        
        case 0xFB:  // EI
            EI();
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
    uint8_t low = fetch();
    uint8_t high = fetch();
    uint16_t addr = (high << 8) | low;
    uint16_t sp = registers->getSP();

    memory->writeByte(addr, sp & 0xFF);          // Write low byte of SP
    memory->writeByte(addr + 1, (sp >> 8) & 0xFF);  // Write high byte of SP

    // Optionally log this memory write and SP value for ML here
}

void CPU::LD_HL_SP_r8() {
    int8_t r8 = static_cast<int8_t>(fetch());  // Fetch signed 8-bit immediate

    uint16_t sp = registers->getSP();
    uint16_t result = sp + r8;

    registers->setHL(result);

    // Flags update as per GameBoy CPU spec:
    // - Z flag = 0
    // - N flag = 0
    // - H flag = set if lower nibble of (SP & 0xF) + (r8 & 0xF) > 0xF
    // - C flag = set if lower byte of SP + r8 > 0xFF

    // Calculate flags
    uint8_t sp_low = sp & 0xFF;
    uint8_t r8_low = r8 & 0xFF;

    // Half carry check
    bool half_carry = ((sp_low & 0xF) + (r8_low & 0xF)) > 0xF;
    // Carry check
    bool carry = (sp_low + r8_low) > 0xFF;

    registers->setFlagZ(false);
    registers->setFlagN(false);
    registers->setFlagH(half_carry);
    registers->setFlagC(carry);

    // Optionally log:
    // - Input SP and r8
    // - Output HL
    // - Flags changes for ML purposes
}

void CPU::LD_SP_HL() {
    uint16_t hl = registers->getHL();
    registers->setSP(hl);

    // This instruction copies HL register pair into SP directly.
    // There are no flags affected.

    // Optionally log the input HL and output SP values for ML training.
}

// 8-bit Load instruction stubs

void CPU::LD_pRR_r(Reg16 rr, Reg8 r) {
    uint16_t addr;

    switch (rr) {
        case Reg16::BC: addr = registers->getBC(); break;
        case Reg16::DE: addr = registers->getDE(); break;
        case Reg16::HL: addr = registers->getHL(); break;
        case Reg16::SP:
            // SP rarely used here for memory indirect load/store,
            // might handle or assert if usage expected
            addr = registers->getSP();
            break;
        default:
            addr = 0;
            break;
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
        default:
            val = 0;
            break;
    }

    memory->writeByte(addr, val);

    // ML logging:
    // - Log input: address from rr, value from register r
    // - Log effect: memory write at addr
}
void CPU::LD_r_pRR(Reg8 r, Reg16 rr) {
    uint16_t addr;

    switch (rr) {
        case Reg16::BC: addr = registers->getBC(); break;
        case Reg16::DE: addr = registers->getDE(); break;
        case Reg16::HL: addr = registers->getHL(); break;
        case Reg16::SP:
            // Usually SP not used here, handle or assert as needed
            addr = registers->getSP();
            break;
        default:
            addr = 0;
            break;
    }

    uint8_t val = memory->readByte(addr);

    // Set register r with value read from memory
    switch (r) {
        case Reg8::A: registers->setA(val); break;
        case Reg8::B: registers->setB(val); break;
        case Reg8::C: registers->setC(val); break;
        case Reg8::D: registers->setD(val); break;
        case Reg8::E: registers->setE(val); break;
        case Reg8::H: registers->setH(val); break;
        case Reg8::L: registers->setL(val); break;
        default:
            // Handle error or unsupported register
            break;
    }

    // Optionally log:
    // - Input: Memory address and data read
    // - Output: Register affected
    // - Entire CPU state snapshot for ML dataset
}

void CPU::LD_r_r(Reg8 dst, Reg8 src) {
    uint8_t val = 0;

    // Read source register value
    switch (src) {
        case Reg8::A: val = registers->getA(); break;
        case Reg8::B: val = registers->getB(); break;
        case Reg8::C: val = registers->getC(); break;
        case Reg8::D: val = registers->getD(); break;
        case Reg8::E: val = registers->getE(); break;
        case Reg8::H: val = registers->getH(); break;
        case Reg8::L: val = registers->getL(); break;
        default:
            // Handle error if needed
            break;
    }

    // Write to destination register
    switch (dst) {
        case Reg8::A: registers->setA(val); break;
        case Reg8::B: registers->setB(val); break;
        case Reg8::C: registers->setC(val); break;
        case Reg8::D: registers->setD(val); break;
        case Reg8::E: registers->setE(val); break;
        case Reg8::H: registers->setH(val); break;
        case Reg8::L: registers->setL(val); break;
        default:
            // Handle error if needed
            break;
    }

    // No flags affected for this instruction.

    // Optionally log input registers (dst, src values) and output register for ML training.
}

void CPU::LDH_pa8_a() {
    uint8_t offset = fetch();  // Fetch 8-bit immediate offset a8
    uint16_t addr = 0xFF00 + offset;

    // Usually the source register is A (the only supported variant for LDH (a8),r),
    // but your system may extend this with parameterization as needed.
    // Here, assuming the value to store is always from A.

    uint8_t val = registers->getA();

    memory->writeByte(addr, val);

    // ML logging suggestion:
    // - Input: PC, immediate offset, register A value
    // - Output: Memory address and byte written (addr, val)
}

void CPU::LDH_a_pa8(Reg8 r) {
    uint8_t offset = fetch();              // Fetch immediate 8-bit offset a8
    uint16_t addr = 0xFF00 + offset;      // Compute high RAM address

    uint8_t val = memory->readByte(addr); // Read byte from memory

    // Write to destination register a
    registers->setA(val); break;

    // No flags are affected by this instruction.

    // Optional ML logging:
    // - Input: PC, offset, memory address, value read
    // - Output: Register r updated value
}

void CPU::LD_A_pC() {
    // Address formed as 0xFF00 + C register (high RAM / IO ports)
    uint16_t addr = 0xFF00 | registers->getC();

    // Read from memory at addr and store in A
    uint8_t val = memory->readByte(addr);
    registers->setA(val);

    // Log read from memory addr and write to A for ML dataset
}

void CPU::LD_pC_A() {
    // Compute I/O port address as 0xFF00 + contents of register C
    uint16_t addr = 0xFF00 | registers->getC();

    // Read value from register A
    uint8_t val = registers->getA();

    // Write value to computed I/O address in memory
    memory->writeByte(addr, val);

    // TODO: Log memory write and register read details for ML dataset

    // No flags are affected in this instruction
}

void CPU::LD_pa16_A() {
    // Fetch low and high bytes of 16-bit immediate address
    uint8_t low = fetch();
    uint8_t high = fetch();
    uint16_t addr = (high << 8) | low;

    // Get value from register A
    uint8_t val = registers->getA();

    // Write value to memory at addr
    memory->writeByte(addr, val);

    // TODO: Log memory write and register read for ML dataset

    // Flags unaffected
}

void CPU::LD_A_pa16() {
    // Fetch low and high bytes of 16-bit immediate address
    uint8_t low = fetch();
    uint8_t high = fetch();
    uint16_t addr = (high << 8) | low;

    // Read byte from memory at addr
    uint8_t val = memory->readByte(addr);

    // Store value into register A
    registers->setA(val);

    // TODO: Log memory read and register write for ML dataset

    // Flags unaffected
}

// Miscellaneous instruction stubs

void CPU::NOP() {
    // Do nothing, no state changes
    
    // Log NOP execution for ML dataset if needed
    
    // Increment cycles as per specs (usually 4 cycles)
    //cycles += 4;
}

void CPU::DI() {
    // Set the Interrupt Master Enable flag to false to disable interrupts
    ime = false;

    // DI does not affect any flags or registers
    
    // Optionally log this event for ML dataset

    // DI instruction consumes 4 cycles
    //cycles += 4;
}
void CPU::EI() {
    // EI enables interrupts but only after the next instruction completes
    // This involves setting a delayed enable flag internally

    // Set a deferred interrupt enable flag to enable IME after the next instruction
    imePending = true;

    // Optionally log this event for ML dataset

    // EI instruction takes 4 cycles
    //cycles += 4;
}

void CPU::STOP() {
    // STOP 0 opcode halts the CPU until a button press or similar event
    // This typically puts CPU into a very low power state awaiting input,
    // for the emulator you can set a flag or handle it at a higher level.

    // Example implementation:
    halted = true;  // Set CPU halted flag (or use a dedicated stop flag if preferred)

    // In actual hardware, STOP also involves the "stop mode" bit in the timer,
    // but for emulator basic behavior halting is often sufficient.

    // 2 machine cycles are consumed by STOP instruction
    //cycles += 2;

    // TODO: Add ML logging of STOP event and CPU state

    // To resume CPU, external event (like input) must clear halted state externally
}

void CPU::HALT() {
    // Set the CPU halted flag so that the CPU stops executing until an interrupt occurs
    halted = true;

    // If IME (interrupt master enable) is set, the CPU will wake on interrupt
    // Processor stops fetching instructions but internal clocks continue ticking.

    // Optionally log interrupt state and HALT event for ML dataset

    // HALT instruction timing is 4 cycles
    //cycles += 4;
}
// Stack

void CPU::POP_rr(Reg16 reg) {
    // Get current stack pointer
    uint16_t sp = registers->getSP();

    // Read low and high bytes from stack memory
    uint8_t low = memory->readByte(sp);
    uint8_t high = memory->readByte(sp + 1);

    // Compose 16-bit value
    uint16_t value = (high << 8) | low;

    // Increment stack pointer by 2 (stack grows down)
    registers->setSP(sp + 2);

    // Store popped value into appropriate register pair
    switch (reg) {
        case Reg16::BC:
            registers->setBC(value);
            break;
        case Reg16::DE:
            registers->setDE(value);
            break;
        case Reg16::HL:
            registers->setHL(value);
            break;
        case Reg16::AF:
            // Mask lower nibble of F because flags F lower bits unused
            registers->setAF(value & 0xFFF0);
            break;
        default:
            // Handle invalid reg case, log error or throw
            break;
    }

    // TODO: Log memory read (sp, sp+1), register write (reg, value) for ML dataset

    // POP consumes 12 machine cycles (typical timing)
    //cycles += 12;
}

void CPU::PUSH_rr(Reg16 reg) {
    // Decrement stack pointer by 2 (stack grows downwards)
    uint16_t sp = registers->getSP() - 2;
    registers->setSP(sp);

    uint16_t value = 0;

    // Get the relevant 16-bit register value to push
    switch (reg) {
        case Reg16::BC:
            value = registers->getBC();
            break;
        case Reg16::DE:
            value = registers->getDE();
            break;
        case Reg16::HL:
            value = registers->getHL();
            break;
        case Reg16::AF:
            // Mask the lower nibble of F, flags should have lower 4 bits 0
            value = registers->getAF() & 0xFFF0;
            break;
        default:
            // Handle invalid reg with error log or exception
            break;
    }

    // Write high byte first to memory at SP + 1 (stack is big-endian)
    memory->writeByte(sp + 1, (value >> 8) & 0xFF);

    // Write low byte to memory at SP
    memory->writeByte(sp, value & 0xFF);

    // TODO: Log memory writes, stack pointer update, register reads for ML dataset

    // PUSH consumes 16 machine cycles on GameBoy CPU
    //cycles += 16;
}

//JR Jumps

void CPU::JR_Nr_r8(Condition nr) {
    // Fetch 8-bit signed offset from the next PC location
    int8_t offset = static_cast<int8_t>(fetch());

    // Save PC before jump for logging
    uint16_t pcBefore = registers->getPC();

    // Evaluate condition based on flags
    bool conditionTrue = false;
    switch (nr) {
        case Condition::NZ:
            conditionTrue = !registers->getFlagZ();
            break;
        case Condition::Z:
            conditionTrue = registers->getFlagZ();
            break;
        case Condition::NC:
            conditionTrue = !registers->getFlagC();
            break;
        case Condition::C:
            conditionTrue = registers->getFlagC();
            break;
    }

    if (conditionTrue) {
        // Taken: relative jump by adding offset to current PC
        registers->setPC(pcBefore + offset);
        // This adds a conditional cycle cost of 4 extra cycles for taken branch (total usually 12)
        //cycles += 12; 
    } else {
        // Not taken - no jump, just normal PC progression done by fetch()
        // Usually 8 cycles consumed if not taken
        //cycles += 8;
    }

    // TODO: Log condition flags, PC before, PC after, offset, and taken/not taken for ML data

}


void CPU::JR_r8() {
    // Fetch 8-bit signed relative offset to add to current PC
    int8_t offset = static_cast<int8_t>(fetch());

    // Record PC before jump for logging
    uint16_t pcBefore = registers->getPC();

    // Compute new PC by adding signed offset
    uint16_t newPC = pcBefore + offset;

    // Set PC to jump target
    registers->setPC(newPC);

    // Log PC update, offset, and jump target for ML dataset here

    // JR r8 unconditional takes 12 cycles
    //cycles += 12;
}

// JP Jumps: Categories 25-28

void CPU::JP_Nr_pa16(Condition cond) {
    // Fetch 16-bit immediate address from PC (low byte first)
    uint8_t low = fetch();
    uint8_t high = fetch();
    uint16_t address = (high << 8) | low;

    // Evaluate condition based on flags
    bool jump = false;
    switch (cond) {
        case Condition::NZ:
            jump = !registers->getFlagZ();
            break;
        case Condition::Z:
            jump = registers->getFlagZ();
            break;
        case Condition::NC:
            jump = !registers->getFlagC();
            break;
        case Condition::C:
            jump = registers->getFlagC();
            break;
    }

    if (jump) {
        // Jump taken: set PC to immediate 16-bit address
        registers->setPC(address);

        // Taken condition: typical of 16 cycles (4 machine cycles)
        //cycles += 16;
    } else {
        // Not taken: PC already advanced by fetching immediate (no jump)
        // Typical timing: 12 cycles (3 machine cycles)
        //cycles += 12;
    }

    // TODO: Log condition flags, address fetched, PC before/after, and branch taken for ML
}

void CPU::JP_HL() {
    // Get target address from HL register pair
    uint16_t addr = registers->getHL();

    // Set PC to HL
    registers->setPC(addr);

    // No flags are affected

    // Typical JP (HL) instruction takes 4 machine cycles (16 cycles total)
    //cycles += 4;

    // TODO: Log PC jump from old PC to new PC (HL) for ML dataset
}

void CPU::JP_a16() {
    // Fetch low and high bytes of 16-bit address
    uint8_t low = fetch();
    uint8_t high = fetch();
    uint16_t addr = (high << 8) | low;

    // Set PC to the fetched address
    registers->setPC(addr);

    // Typical JP a16 timing is 16 cycles (4 machine cycles)
    //cycles += 16;

    // TODO: Log PC jump and target address for ML dataset
}

// CALL Instructions: Categories 29-31

void CPU::CALL_Nr_a16(Condition cond) {
    // Fetch 16-bit immediate address (low byte, then high byte)
    uint8_t low = fetch();
    uint8_t high = fetch();
    uint16_t addr = (high << 8) | low;

    // Evaluate the condition based on the CPU flags
    bool conditionMet = false;
    switch (cond) {
        case Condition::NZ:
            conditionMet = !registers->getFlagZ();
            break;
        case Condition::Z:
            conditionMet = registers->getFlagZ();
            break;
        case Condition::NC:
            conditionMet = !registers->getFlagC();
            break;
        case Condition::C:
            conditionMet = registers->getFlagC();
            break;
    }

    if (conditionMet) {
        // Push return address (PC after instruction, i.e. current PC) onto stack

        // PC is currently after reading the 16-bit immediate — PC points to next instruction
        uint16_t returnAddr = registers->getPC();

        // Decrement SP by 2 to make space on stack
        uint16_t sp = registers->getSP() - 2;
        registers->setSP(sp);

        // Write high and low bytes of return address to stack (big-endian)
        memory->writeByte(sp, returnAddr & 0xFF);
        memory->writeByte(sp + 1, (returnAddr >> 8) & 0xFF);

        // Set PC to target address (call)
        registers->setPC(addr);

        // CALL taken usually costs 24 cycles (6 machine cycles)
        cycles += 24;
    } else {
        // Call not taken — PC already advanced past operand bytes, so do nothing special
        // Typical timing for conditional call not taken: 12 cycles (3 machine cycles)
        cycles += 12;
    }

    // TODO: Add ML logging capturing CPU flags, PC before and after, stack pointer changes,
    //       address called, and branch taken/not taken for training
}

void CPU::CALL_a16() {
    // Fetch the 16-bit immediate address from instruction stream (low byte first)
    uint8_t low = fetch();
    uint8_t high = fetch();
    uint16_t addr = (high << 8) | low;

    // PC currently points after these two bytes, which is the return address

    // Compute return address (current PC)
    uint16_t returnAddr = registers->getPC();

    // Decrement SP by 2 to allocate stack space
    uint16_t sp = registers->getSP() - 2;
    registers->setSP(sp);

    // Push return address to stack (big-endian)
    memory->writeByte(sp, returnAddr & 0xFF);         // Low byte
    memory->writeByte(sp + 1, (returnAddr >> 8) & 0xFF); // High byte

    // Set PC to target call address
    registers->setPC(addr);

    // Typical CALL a16 timing is 24 cycles (6 machine cycles)
    //cycles += 24;

    // TODO: Log stack pointer update, memory writes, PC change for ML dataset
}


// RET Instructions: Categories 32-35

void CPU::RET_Nr(Condition cond) {
    // Evaluate the condition based on CPU flags
    bool conditionMet = false;
    switch (cond) {
        case Condition::NZ:
            conditionMet = !registers->getFlagZ();
            break;
        case Condition::Z:
            conditionMet = registers->getFlagZ();
            break;
        case Condition::NC:
            conditionMet = !registers->getFlagC();
            break;
        case Condition::C:
            conditionMet = registers->getFlagC();
            break;
    }

    if (conditionMet) {
        // Pop 16-bit return address from stack
        uint16_t sp = registers->getSP();
        uint8_t low = memory->readByte(sp);
        uint8_t high = memory->readByte(sp + 1);
        uint16_t retAddr = (high << 8) | low;

        // Increment stack pointer by 2
        registers->setSP(sp + 2);

        // Set PC to popped address
        registers->setPC(retAddr);

        // Cycle cost when taken: typically 20 cycles (5 machine cycles)
        cycles += 20;
    } else {
        // Not taken: PC already advanced past RET opcode by fetch()
        // Cycle cost when not taken: typically 8 cycles (2 machine cycles)
        cycles += 8;
    }

    // TODO: 
    // - Log condition evaluated and result
    // - Log stack reads and SP update if taken
    // - Log PC before and after instruction for ML training
}

void CPU::RET() {
    // Read the low and high bytes of the return address from stack pointer (SP)
    uint16_t sp = registers->getSP();
    uint8_t low = memory->readByte(sp);
    uint8_t high = memory->readByte(sp + 1);
    uint16_t returnAddr = (high << 8) | low;

    // Increment stack pointer by 2 after popping address
    registers->setSP(sp + 2);

    // Update PC to the return address (pop)
    registers->setPC(returnAddr);

    // Typical RET instruction consumes 16 cycles (4 machine cycles)
    cycles += 16;

    // TODO: Log stack read, SP update, PC change for ML dataset
}

void CPU::RETI() {
    // Pop 16-bit return address from stack (little endian)
    uint16_t sp = registers->getSP();
    uint8_t low = memory->readByte(sp);
    uint8_t high = memory->readByte(sp + 1);
    uint16_t retAddr = (high << 8) | low;

    // Increment SP by 2 after popping
    registers->setSP(sp + 2);

    // Set PC to popped return address
    registers->setPC(retAddr);

    // Enable interrupts by setting IME flag
    ime = true;

    // Typical RETI instruction timing: 16 cycles (4 machine cycles)
    cycles += 16;

    // TODO: Log stack reads, SP update, PC change, and IME flag set for ML dataset
}

// 16-bit Arithmetic Instructions: Categories 38-41

void CPU::ADD_HL_rr(Reg16 reg) {
    uint16_t hl = registers->getHL();
    uint16_t value = 0;

    // Select target 16-bit register pair
    switch (reg) {
        case Reg16::BC:
            value = registers->getBC();
            break;
        case Reg16::DE:
            value = registers->getDE();
            break;
        case Reg16::HL:
            value = registers->getHL();
            break;
        case Reg16::SP:
            value = registers->getSP();
            break;
        default:
            // Handle invalid reg case (log or throw)
            return;
    }

    uint32_t result = hl + value;  // Use wider type for detecting carry

    // Update HL with lower 16 bits of the result
    registers->setHL(static_cast<uint16_t>(result));

    // Flags update according to GameBoy CPU spec for ADD HL,rr:
    // - N flag reset (0)
    // - H flag set if carry from bit 11 (0x0FFF + low nibble)
    // - C flag set if carry from bit 15 (16-bit overflow)

    // Half carry: if (lower 12 bits of hl + lower 12 bits of value) > 0xFFF
    bool halfCarry = ((hl & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF;

    // Carry: if result > 0xFFFF
    bool carry = (result & 0x10000) != 0;

    // Set flags accordingly
    registers->setFlagN(false);
    registers->setFlagH(halfCarry);
    registers->setFlagC(carry);

    // This instruction typically takes 8 machine cycles (16 clock cycles)
    cycles += 8;

    // TODO: Log input HL & rr values, result, flags set/cleared, and cycle count for ML
}

void CPU::ADD_SP_r8() {
    // Fetch signed 8-bit immediate operand
    int8_t r8 = static_cast<int8_t>(fetch());

    uint16_t sp = registers->getSP();
    uint16_t result = sp + r8;

    registers->setSP(result);

    // Flags update per GameBoy CPU spec for ADD SP, r8:
    // Z flag cleared
    // N flag cleared
    // H flag set if carry from bit 3 (lower nibble)
    // C flag set if carry from bit 7 (lower byte)

    // Extract low bytes to check half-carry and carry
    uint8_t sp_low = sp & 0xFF;
    uint8_t r8_low = static_cast<uint8_t>(r8);

    // Half-carry occurs if ((sp_low & 0xF) + (r8_low & 0xF)) > 0xF
    bool half_carry = ((sp_low & 0x0F) + (r8_low & 0x0F)) > 0x0F;

    // Carry occurs if (sp_low + r8_low) > 0xFF
    bool carry = (static_cast<uint16_t>(sp_low) + static_cast<uint16_t>(r8_low)) > 0xFF;

    registers->setFlagZ(false);
    registers->setFlagN(false);
    registers->setFlagH(half_carry);
    registers->setFlagC(carry);

    // Typical instruction timing: 16 cycles (4 machine cycles)
    cycles += 16;

    // TODO: Log input SP, r8, result SP, and flag values for ML dataset
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

//Prefix CB

void CPU::PrefixCB() {
    // Fetch the next opcode byte to select CB-prefixed instruction
    uint8_t cbOpcode = fetch();

    // Example switch for CB-prefixed instructions (expand as needed)
    switch (cbOpcode) {
        case 0x00: // RLC B
            RLC_r(Reg8::B);
            break;
        case 0x01: // RLC C
            RLC_r(Reg8::C);
            break;
        // ... add all other CB-prefixed instructions with proper Reg8/bit parameters

        default:
            // Handle invalid CB opcode or log error
            break;
    }

    // Update cycles according to each CB instruction specification

    // Optionally log CB prefix and instruction for ML dataset
}