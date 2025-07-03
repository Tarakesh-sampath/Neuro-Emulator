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
        
        case 0x03:  // INC BC
            INC_rr(Reg16::BC);
            break;
        
        case 0x04:  // INC B
            INC_r(Reg8::B);
            break;
            
        case 0x05:  // DEC B
            DEC_r(Reg8::B);
            break;
        
        case 0x06:  // LD B,d8
            LD_r_d8(Reg8::B);
            break;
            
        case 0x07:  // RLCA
            RLCA();
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
        
        case 0x0B:  // DEC BC
            DEC_rr(Reg16::BC);
            break;
        
        case 0x0C:  // INC C
            INC_r(Reg8::C);
            break;
            
        case 0x0D:  // DEC C
            DEC_r(Reg8::C);
            break;
        
        case 0x0E:  // LD C,d8
            LD_r_d8(Reg8::C);
            break;
                    
        case 0x0F:  // RRCA
            RRCA();
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
            
        case 0x13:  // INC DE
            INC_rr(Reg16::DE);
            break;

        case 0x14:  // INC D
            INC_r(Reg8::D);
            break;

        case 0x15:  // DEC D
            DEC_r(Reg8::D);
            break;
                
        case 0x16:  // LD D,d8
            LD_r_d8(Reg8::D);
            break;
        
        case 0x17:  // RLA
            RLA();
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
        
        case 0x1B:  // DEC DE
            DEC_rr(Reg16::DE);
            break;
    
        case 0x1C:  // INC E
            INC_r(Reg8::E);
            break;

        case 0x1D:  // DEC E
            DEC_r(Reg8::E);
            break;
                    
        case 0x1E:  // LD E,d8
            LD_r_d8(Reg8::E);
            break;
                    
        case 0x1F:  // RRA
            RRA();
            break;
            
        case 0x20:  // JR NZ,r8
            JR_Nr_r8(Condition::NZ);
            break;
            
        case 0x21: // LD HL,d16
            LD_rr_d16(Reg16::HL);
            break;
        
        case 0x22:  // LD (HL+), A
            LD_pHL_inc_A();
            break;

        case 0x23:  // INC HL
            INC_rr(Reg16::HL);
            break;
            
        case 0x24:  // INC H
            INC_r(Reg8::H);
            break;
                    
        case 0x25:  // DEC H
            DEC_r(Reg8::H);
            break;
        
        case 0x26:  // LD H,d8
            LD_r_d8(Reg8::H);
            break;
                
        case 0x27:  // DAA
            DAA();
            break;

        case 0x28:  // JR Z,r8
            JR_Nr_r8(Condition::Z);
            break;
                        
        case 0x29:  // ADD HL, HL
            ADD_HL_rr(Reg16::HL);
            break;
        
        case 0x2A:  // LD A,(HL+)
            LD_A_pHL_inc();
            break;
            
        case 0x2B:  // DEC HL
            DEC_rr(Reg16::HL);
            break;

        case 0x2C:  // INC L
            INC_r(Reg8::L);
            break;

        case 0x2D:  // DEC L
            DEC_r(Reg8::L);
            break;
        
        case 0x2E:  // LD L,d8
            LD_r_d8(Reg8::L);
            break;
                    
        case 0x2F:  // CPL
            CPL();
            break;
        
        case 0x30:  // JR NC,r8
            JR_Nr_r8(Condition::NC);
            break;
            
        case 0x31: // LD SP,d16
            LD_rr_d16(Reg16::SP);
            break;
        
        case 0x32:  // LD (HL-), A
            LD_pHL_dec_A();
            break;
            
        case 0x33:  // INC SP
            INC_rr(Reg16::SP);
            break;
        
        case 0x34:  // INC (HL)
            INC_pHL();
            break;
            
        case 0x35:  // DEC (HL)
            DEC_pHL();
            break;        
        
        case 0x36:  // LD (HL), d8
            LD_pHL_d8();
            break;
        
        case 0x37:  // SCF
            SCF();
            break;

        case 0x38:  // JR C,r8
            JR_Nr_r8(Condition::C);
            break;
            
        case 0x39:  // ADD HL, SP
            ADD_HL_rr(Reg16::SP);
            break;
            
        case 0x3A:  // LD A,(HL-)
            LD_A_pHL_dec();
            break;
        
        case 0x3B:  // DEC SP
            DEC_rr(Reg16::SP);
            break;
        
        case 0x3C:  // INC A
            INC_r(Reg8::A);
            break;
        
        case 0x3D:  // DEC A
            DEC_r(Reg8::A);
            break;
        
        case 0x3E:  // LD A,d8
            LD_r_d8(Reg8::A);
            break;
            
        case 0x3F:  // CCF
            CCF();
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
            
        case 0x46: // LD B,(HL)
            LD_r_pRR(Reg8::B, Reg16::HL);
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

        case 0x4E: // LD C,(HL)
            LD_r_pRR(Reg8::C, Reg16::HL);
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
            
        case 0x56: // LD D,(HL)
            LD_r_pRR(Reg8::D, Reg16::HL);
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
            
        case 0x5E: // LD E,(HL)
            LD_r_pRR(Reg8::E, Reg16::HL);
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
            
        case 0x66: // LD H,(HL)
            LD_r_pRR(Reg8::H, Reg16::HL);
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
        
        case 0x6E: // LD L,(HL)
            LD_r_pRR(Reg8::L, Reg16::HL);
            break;
            
        case 0x6F: // LD L A
            LD_r_r(Reg8::L, Reg8::A); 
            break;
           
        case 0x70:  // LD (HL), B 
            LD_pRR_r(Reg16::HL, Reg8::B);
            break;
        
        case 0x71:  // LD (HL), C
            LD_pRR_r(Reg16::HL, Reg8::C);
            break;
            
        case 0x72:  // LD (HL), D
            LD_pRR_r(Reg16::HL, Reg8::D);
            break;
        
        case 0x73:  // LD (HL), E
            LD_pRR_r(Reg16::HL, Reg8::E);
            break;
        
        case 0x74:  // LD (HL), H 
            LD_pRR_r(Reg16::HL, Reg8::H);
            break;
            
        case 0x75:  // LD (HL), L
            LD_pRR_r(Reg16::HL, Reg8::L);
            break;
                 
        case 0x76:  // HALT
            HALT();
            break;
            
        case 0x77:  // LD (HL), A 
            LD_pRR_r(Reg16::HL, Reg8::A);
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
        
        case 0x80: // ADD B
            ADD_r(Reg8::B);
            break;
        
        case 0x81: // ADD C
            ADD_r(Reg8::C);
            break;
            
        case 0x82: // ADD D
            ADD_r(Reg8::D);
            break;
            
        case 0x83: // ADD E
            ADD_r(Reg8::E);
            break;
        
        case 0x84: // ADD H
            ADD_r(Reg8::H);
            break;
            
        case 0x85: // ADD l
            ADD_r(Reg8::L);
            break;
        
        case 0x86: // ADD (HL)
            ADD_pHL();
            break; 
            
        case 0x87: // ADD A
            ADD_r(Reg8::A);
            break;
        
        case 0x88:  // ADC A,B
            ADC_r(Reg8::B);
            break;
            
        case 0x89:  // ADC A,C
            ADC_r(Reg8::C);
            break;
            
        case 0x8A:  // ADC A,D
            ADC_r(Reg8::D);
            break;
            
        case 0x8B:  // ADC A,E
            ADC_r(Reg8::E);
            break;
            
        case 0x8C:  // ADC A,H
            ADC_r(Reg8::H);
            break;
            
        case 0x8D:  // ADC A,L
            ADC_r(Reg8::L);
            break;
        
        case 0x8E:  // ADC A,(HL)
            ADC_pHL();
            break;
            
        case 0x8F:  // ADC A,A
            ADC_r(Reg8::A);
            break;
            
        case 0x90: //SUB B
            SUB_r(Reg8::B);
            break;
        
        case 0x91: //SUB C
            SUB_r(Reg8::C);
            break;
        
        case 0x92: //SUB D
            SUB_r(Reg8::D);
            break;
        
        case 0x93: //SUB E
            SUB_r(Reg8::E); 
            break;
        
        case 0x94: //SUB H
            SUB_r(Reg8::H);
            break;
        
        case 0x95: //SUB L
            SUB_r(Reg8::L);
            break;
        
        case 0x96: // SUB (HL)
            SUB_pHL();
            break;  
            
        case 0x97: //SUB A
            SUB_r(Reg8::A);
            break;
            
        case 0x98:  // SBC A,B
            SBC_r(Reg8::B);
            break;
        
        case 0x99:  // SBC A,C
            SBC_r(Reg8::C);
            break;
        
        case 0x9A:  // SBC A,D
            SBC_r(Reg8::D);
            break;
        
        case 0x9B:  // SBC A,E
            SBC_r(Reg8::E);
            break;
        
        case 0x9C:  // SBC A,H
            SBC_r(Reg8::H);
            break;
        
        case 0x9D:  // SBC A,L
            SBC_r(Reg8::L);
            break;
        
        case 0x9E:  // SBC A,(HL)
            SBC_pHL();
            break;
            
        case 0x9F:  // SBC A,A
            SBC_r(Reg8::A);
            break;
        
        case 0xA0:  // AND B
            AND_r(Reg8::B);
            break;
            
        case 0xA1:  // AND C
            AND_r(Reg8::C);
            break;
            
        case 0xA2:  // AND D
            AND_r(Reg8::D);
            break;
            
        case 0xA3:  // AND E
            AND_r(Reg8::E);
            break;
            
        case 0xA4:  // AND H
            AND_r(Reg8::H);
            break;
            
        case 0xA5:  // AND L
            AND_r(Reg8::L);
            break;

        case 0xA6:  // AND (HL)
            AND_pHL();
            break;
            
        case 0xA7:  // AND A
            AND_r(Reg8::A);
            break;
        
        case 0xA8:  // XOR B
            XOR_r(Reg8::B);
            break;
            
        case 0xA9:  // XOR C
            XOR_r(Reg8::C);
            break;
            
        case 0xAA:  // XOR D
            XOR_r(Reg8::D);
            break;
            
        case 0xAB:  // XOR E
            XOR_r(Reg8::E);
            break;
            
        case 0xAC:  // XOR H
            XOR_r(Reg8::H);
            break;
            
        case 0xAD:  // XOR L
            XOR_r(Reg8::L);
            break;
        
        case 0xAE:  // XOR (HL)
            XOR_pHL();
            break;

        case 0xAF:  // XOR A
            XOR_r(Reg8::A);
            break;
            
        case 0xB0:  // OR B
            OR_r(Reg8::B);
            break;

        case 0xB1:  // OR C
            OR_r(Reg8::C);
            break;

        case 0xB2:  // OR D
            OR_r(Reg8::D);
            break;

        case 0xB3:  // OR E
            OR_r(Reg8::E);
            break;

        case 0xB4:  // OR H
            OR_r(Reg8::H);
            break;

        case 0xB5:  // OR L
            OR_r(Reg8::L);
            break;
        
        case 0xB6:  // OR (HL)
            OR_pHL();
            break;

        case 0xB7:  // OR A
            OR_r(Reg8::A);
            break;
        
        case 0xB8:  // CP B
            CP_r(Reg8::B);
            break;
            
        case 0xB9:  // CP C
            CP_r(Reg8::C);
            break;
            
        case 0xBA:  // CP D
            CP_r(Reg8::D);
            break;
        
        case 0xBB:  // CP E
            CP_r(Reg8::E);
            break;
        
        case 0xBC:  // CP H
            CP_r(Reg8::H);
            break;
        
        case 0xBD:  // CP L
            CP_r(Reg8::L);
            break;
            
        case 0xBE:  // CP (HL)
            CP_pHL();
            break;
        
        case 0xBF:  // CP A
            CP_r(Reg8::A);
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
        
        case 0xC6:  // ADD A,d8
            ADD_r8();
            break;
            
        case 0xC7:  // RST 00H
            RST(0x00);
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
        
        case 0xCE:  // ADC A, d8
            ADC_r8();
            break;
        
        case 0xCF:  // RST 08H
            RST(0x08);
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
                
        case 0xD6:  // SUB d8
            SUB_r8();
            break;
        
        case 0xD7:  // RST 10H
            RST(0x10);
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
           
        case 0xDE:  // SBC A, d8
            SBC_d8();
            break;
            
        case 0xDF:  // RST 18H
            RST(0x18);
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
        
        case 0xE6:  // AND d8
            AND_d8();
            break;
            
        case 0xE7:  // RST 12H
            RST(0x20);
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
        
        case 0xEE:  // XOR d8
            XOR_d8();
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
        
        case 0xF6:  // OR d8
            OR_d8();
            break;
        
        case 0xF7:  // RST 12H
            RST(0x30);
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
        
        case 0xFE:  // CP d8
            CP_d8();
            break;
        
        case 0xFF:  // RST 12H
            RST(0x38);
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

void CPU::LD_r_d8(Reg8 r) {
    uint8_t val = fetch();  // Fetch immediate 8-bit operand

    switch (r) {
        case Reg8::A: registers->setA(val); break;
        case Reg8::B: registers->setB(val); break;
        case Reg8::C: registers->setC(val); break;
        case Reg8::D: registers->setD(val); break;
        case Reg8::E: registers->setE(val); break;
        case Reg8::H: registers->setH(val); break;
        case Reg8::L: registers->setL(val); break;
        default: 
            // Defensive: handle invalid register if needed
            break;
    }

    // No flags are affected

    // Optionally log input immediate, target reg write for ML training
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

void CPU::LDH_a_pa8() {
    uint8_t offset = fetch();              // Fetch immediate 8-bit offset a8
    uint16_t addr = 0xFF00 + offset;      // Compute high RAM address

    uint8_t val = memory->readByte(addr); // Read byte from memory

    // Write to destination register a
    registers->setA(val);

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

void CPU::LD_A_pHL_inc() {
    uint16_t addr = registers->getHL();
    uint8_t val = memory->readByte(addr);
    registers->setA(val);
    registers->setHL(addr + 1);

    // Log read from memory addr, register A write, HL increment for ML dataset here
}

void CPU::LD_A_pHL_dec() {
    uint16_t addr = registers->getHL();
    uint8_t val = memory->readByte(addr);
    registers->setA(val);
    registers->setHL(addr - 1);

    // Log read from memory addr, register A write, HL decrement for ML dataset here
}

void CPU::LD_pHL_inc_A() {
    uint16_t addr = registers->getHL();
    uint8_t val = registers->getA();

    memory->writeByte(addr, val);
    registers->setHL(addr + 1);

    // Log for ML:
    // - Memory write address and value (addr, val)
    // - HL before and after increment
    // - Register A value
}

void CPU::LD_pHL_dec_A() {
    uint16_t addr = registers->getHL();
    uint8_t val = registers->getA();

    memory->writeByte(addr, val);
    registers->setHL(addr - 1);

    // Log for ML:
    // - Memory write address and value
    // - HL before and after decrement
    // - Register A value
}

void CPU::LD_pHL_d8() {
    uint8_t val = fetch();         // Fetch immediate 8-bit value
    uint16_t addr = registers->getHL();

    memory->writeByte(addr, val);

    // Log for ML:
    // - PC prefetch address
    // - Immediate val
    // - HL pointer (addr)
    // - Memory write effect

    // Note: This instruction does not affect flags
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

void CPU::INC_rr(Reg16 reg) {
    uint16_t val = 0;

    // Get current 16-bit register pair value
    switch (reg) {
        case Reg16::BC:
            val = registers->getBC();
            break;
        case Reg16::DE:
            val = registers->getDE();
            break;
        case Reg16::HL:
            val = registers->getHL();
            break;
        case Reg16::SP:
            val = registers->getSP();
            break;
        default:
            // Handle illegal register case (optionally log or assert)
            return;
    }

    // Increment value by 1
    val++;

    // Write back incremented value
    switch (reg) {
        case Reg16::BC:
            registers->setBC(val);
            break;
        case Reg16::DE:
            registers->setDE(val);
            break;
        case Reg16::HL:
            registers->setHL(val);
            break;
        case Reg16::SP:
            registers->setSP(val);
            break;
    }

    // Note: INC_rr does not affect CPU flags (Z, N, H, C remain unchanged)

    // Typical timing: 8 cycles (2 machine cycles)
    cycles += 8;

    // TODO: Log original value, incremented value, register affected, and cycle count for ML dataset
}

void CPU::DEC_rr(Reg16 reg) {
    uint16_t val = 0;

    // Read current 16-bit register pair value
    switch (reg) {
        case Reg16::BC:
            val = registers->getBC();
            break;
        case Reg16::DE:
            val = registers->getDE();
            break;
        case Reg16::HL:
            val = registers->getHL();
            break;
        case Reg16::SP:
            val = registers->getSP();
            break;
        default:
            // Optionally handle invalid case with assert or error
            return;
    }

    // Decrement value by 1 (wrap-around handled naturally by uint16_t)
    val--;

    // Write updated value back to register pair
    switch (reg) {
        case Reg16::BC:
            registers->setBC(val);
            break;
        case Reg16::DE:
            registers->setDE(val);
            break;
        case Reg16::HL:
            registers->setHL(val);
            break;
        case Reg16::SP:
            registers->setSP(val);
            break;
    }

    // Flags are not affected by DEC rr instruction

    // Typically takes 8 cycles (2 machine cycles)
    cycles += 8;

    // TODO: Log original value, decremented value, register affected, and cycle count for ML
}

// 8-bit Arithmetic Instructions: Categories 42-53

void CPU::ADD_r(Reg8 src) {
    // Only ADD to register A supported per GameBoy spec

    uint8_t aVal = registers->getA();
    uint8_t srcVal;

    // Read src register value
    switch (src) {
        case Reg8::B: srcVal = registers->getB(); break;
        case Reg8::C: srcVal = registers->getC(); break;
        case Reg8::D: srcVal = registers->getD(); break;
        case Reg8::E: srcVal = registers->getE(); break;
        case Reg8::H: srcVal = registers->getH(); break;
        case Reg8::L: srcVal = registers->getL(); break;
        case Reg8::A: srcVal = registers->getA(); break;
        default:
            // Handle error or treat as zero
            srcVal = 0; 
            break;
    }

    uint16_t result = aVal + srcVal;

    // Update register A
    registers->setA(static_cast<uint8_t>(result & 0xFF));

    // Flags update:
    // Z - Set if result zero
    // N - Reset (0) because addition
    // H - Set if carry from bit 3 to bit 4 (half carry)
    // C - Set if carry (result > 0xFF)

    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(false);
    registers->setFlagH(((aVal & 0xF) + (srcVal & 0xF)) > 0xF);
    registers->setFlagC(result > 0xFF);

    // Typical timing: 4 cycles (1 machine cycle)
    cycles += 4;

    // TODO: Log CPU state, input register values, result, flags, and cycles for ML dataset
}

void CPU::ADD_r8() {
    // Fetch 8-bit immediate value
    uint8_t val = fetch();

    uint8_t A = registers->getA();
    uint16_t result = static_cast<uint16_t>(A) + val;

    // Set register A to lower 8 bits of result
    registers->setA(static_cast<uint8_t>(result & 0xFF));

    // Update flags accordingly:
    // Z - Set if result is zero
    // N - Reset (addition operation)
    // H - Set if carry from bit 3 to bit 4 (half carry)
    // C - Set if carry from bit 7 (full carry)
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(false);
    registers->setFlagH(((A & 0xF) + (val & 0xF)) > 0xF);
    registers->setFlagC(result > 0xFF);

    // Typical timing: 8 cycles (2 machine cycles)

    cycles += 8;

    // ML Logging Suggestion:
    // Log inputs: A before, immediate val, flags before
    // Log outputs: A after, flags after, cycle count
}

void CPU::ADD_pHL() {
    // Address in HL register
    uint16_t addr = registers->getHL();

    // Read value from memory at HL
    uint8_t memVal = memory->readByte(addr);

    uint8_t A = registers->getA();
    uint16_t result = A + memVal;

    // Store result back into A
    registers->setA(static_cast<uint8_t>(result & 0xFF));

    // Update flags:
    // Z - Set if result zero
    // N - Reset (addition)
    // H - Set if carry from bit 3 (half carry)
    // C - Set if carry from bit 7 (overflow above 255)

    bool halfCarry = ((A & 0xF) + (memVal & 0xF)) > 0xF;
    bool carry = result > 0xFF;

    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(false);
    registers->setFlagH(halfCarry);
    registers->setFlagC(carry);

    // Typical timing: 8 cycles due to memory access
    cycles += 8;

    // TODO: Log read from memory, A before and after, flags updated for ML training
}

void CPU::SUB_r(Reg8 src) {
    // Read accumulator and source register value
    uint8_t A = registers->getA();
    uint8_t val;

    switch (src) {
        case Reg8::B: val = registers->getB(); break;
        case Reg8::C: val = registers->getC(); break;
        case Reg8::D: val = registers->getD(); break;
        case Reg8::E: val = registers->getE(); break;
        case Reg8::H: val = registers->getH(); break;
        case Reg8::L: val = registers->getL(); break;
        case Reg8::A: val = registers->getA(); break;
        default:
            val = 0;  // Or handle error
            break;
    }

    uint16_t result = static_cast<uint16_t>(A) - static_cast<uint16_t>(val);

    uint8_t result8 = static_cast<uint8_t>(result & 0xFF);

    // Store result back into accumulator A
    registers->setA(result8);

    // Flags update per GameBoy CPU spec for SUB:
    // Z - set if result == 0
    // N - set (subtraction)
    // H - set if borrow from bit 4 (i.e., if (A & 0xF) < (val & 0xF))
    // C - set if borrow (if A < val)

    bool halfBorrow = (A & 0xF) < (val & 0xF);
    bool borrow = A < val;

    registers->setFlagZ(result8 == 0);
    registers->setFlagN(true);
    registers->setFlagH(halfBorrow);
    registers->setFlagC(borrow);

    // Typical timing for SUB r: 4 cycles
    cycles += 4;

    // TODO: Log input registers, result, flags, and cycle count for ML dataset
}

void CPU::SUB_pHL() {
    // Get memory address from HL register pair
    uint16_t addr = registers->getHL();

    // Read the value at memory[HL]
    uint8_t memVal = memory->readByte(addr);

    uint8_t A = registers->getA();

    // Perform subtraction
    uint16_t result = static_cast<uint16_t>(A) - static_cast<uint16_t>(memVal);
    uint8_t result8 = static_cast<uint8_t>(result & 0xFF);

    // Store result back into A
    registers->setA(result8);

    // Update flags according to GameBoy SUB rules:
    // Z - set if result == 0
    // N - set (because this is subtraction)
    // H - set if borrow from bit 4 occurred (half borrow)
    // C - set if full borrow occurred (if A < memVal)

    bool halfBorrow = (A & 0x0F) < (memVal & 0x0F);
    bool borrow = A < memVal;

    registers->setFlagZ(result8 == 0);
    registers->setFlagN(true);
    registers->setFlagH(halfBorrow);
    registers->setFlagC(borrow);

    // Typical timing for SUB (HL) is 8 cycles (memory read penalty)
    cycles += 8;

    // TODO: Log:
    // - Input: A before subtraction, memory value at HL, flags before
    // - Output: A after subtraction, flags updated, cycles
    // for ML training dataset
}

void CPU::SUB_r8() {
    uint8_t value = fetch();  // Fetch immediate 8-bit operand

    uint8_t A = registers->getA();
    uint16_t result = static_cast<uint16_t>(A) - static_cast<uint16_t>(value);
    uint8_t result8 = static_cast<uint8_t>(result & 0xFF);

    // Store result back into accumulator A
    registers->setA(result8);

    // Flags update per GameBoy SUB rules:
    // Z - set if result == 0
    // N - set (because this is subtraction)
    // H - set if borrow from bit 4 occurred (half borrow)
    // C - set if full borrow occurred (if A < value)

    bool halfBorrow = (A & 0xF) < (value & 0xF);
    bool borrow = A < value;

    registers->setFlagZ(result8 == 0);
    registers->setFlagN(true);
    registers->setFlagH(halfBorrow);
    registers->setFlagC(borrow);

    // Typical timing for SUB d8: 8 cycles
    cycles += 8;

    // TODO: Log input register A, immediate value, result, flags, and cycle count for ML dataset
}

void CPU::ADC_r(Reg8 r) {
    // Read source register value
    uint8_t value = 0;
    switch (r) {
        case Reg8::A: value = registers->getA(); break;
        case Reg8::B: value = registers->getB(); break;
        case Reg8::C: value = registers->getC(); break;
        case Reg8::D: value = registers->getD(); break;
        case Reg8::E: value = registers->getE(); break;
        case Reg8::H: value = registers->getH(); break;
        case Reg8::L: value = registers->getL(); break;
        default: value = 0; break; // Defensive fallback
    }

    uint8_t A = registers->getA();
    uint8_t carry = registers->getFlagC() ? 1 : 0;

    // Perform ADC operation: A + value + carry
    uint16_t result = A + value + carry;

    // Set flags according to GameBoy CPU rules
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(false);
    registers->setFlagH(((A & 0xF) + (value & 0xF) + carry) > 0xF);
    registers->setFlagC(result > 0xFF);

    // Write result back to A register
    registers->setA(static_cast<uint8_t>(result & 0xFF));

    // Optional ML logging:
    // Inputs: A before, source value, carry flag
    // Outputs: result A, updated flags (Z,N,H,C)
}

void CPU::ADC_pHL() {
    uint16_t addr = registers->getHL();
    uint8_t value = memory->readByte(addr);
    uint8_t A = registers->getA();
    uint8_t carry = registers->getFlagC() ? 1 : 0;

    // Perform ADC operation: A + value + carry
    uint16_t result = A + value + carry;

    // Set flags according to GameBoy CPU ADC rules
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(false);
    registers->setFlagH(((A & 0xF) + (value & 0xF) + carry) > 0xF);
    registers->setFlagC(result > 0xFF);

    // Write result back to A register
    registers->setA(static_cast<uint8_t>(result & 0xFF));

    // ML logging hooks here: inputs: A, value from mem, carry flag; outputs: A result, flags
}

void CPU::SBC_r(Reg8 r) {
    // Get value of source register
    uint8_t value = 0;
    switch (r) {
        case Reg8::A: value = registers->getA(); break;
        case Reg8::B: value = registers->getB(); break;
        case Reg8::C: value = registers->getC(); break;
        case Reg8::D: value = registers->getD(); break;
        case Reg8::E: value = registers->getE(); break;
        case Reg8::H: value = registers->getH(); break;
        case Reg8::L: value = registers->getL(); break;
        default: value = 0; break; // defensive fallback
    }

    uint8_t A = registers->getA();
    uint8_t carry = registers->getFlagC() ? 1 : 0;

    // Perform SBC operation: A - value - carry
    int16_t result = static_cast<int16_t>(A) - static_cast<int16_t>(value) - carry;

    // Set flags according to GameBoy CPU SBC rules
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(true);
    // Half Carry flag is set if borrow from bit 4
    registers->setFlagH(((A & 0xF) - (value & 0xF) - carry) < 0);
    // Carry if borrow occurs (result < 0)
    registers->setFlagC(result < 0);

    // Write result back to A register (low 8 bits)
    registers->setA(static_cast<uint8_t>(result & 0xFF));

    // Optionally log:
    // Inputs: A before, src value, carry flag before
    // Outputs: result A, updated flags (Z, N, H, C)
}

void CPU::SBC_pHL() {
    uint16_t addr = registers->getHL();
    uint8_t value = memory->readByte(addr);
    uint8_t A = registers->getA();
    uint8_t carry = registers->getFlagC() ? 1 : 0;

    // Perform SBC operation: A - value - carry
    int16_t result = static_cast<int16_t>(A) - static_cast<int16_t>(value) - carry;

    // Set flags according to GameBoy CPU SBC rules
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(true);
    // Half Carry is set if borrow from bit 4
    registers->setFlagH(((A & 0xF) - (value & 0xF) - carry) < 0);
    // Carry if borrow occurs (result < 0)
    registers->setFlagC(result < 0);

    // Write result (low 8 bits) back to A register
    registers->setA(static_cast<uint8_t>(result & 0xFF));

    // ML logging hooks:
    // Inputs: A, memory value at HL, carry flag before operation
    // Outputs: new A, flags Z, N, H, C
}

void CPU::SBC_d8() {
    uint8_t value = fetch();  // Fetch the immediate 8-bit operand

    uint8_t A = registers->getA();
    uint8_t carry = registers->getFlagC() ? 1 : 0;

    // Perform SBC operation: A - value - carry
    int16_t result = static_cast<int16_t>(A) - static_cast<int16_t>(value) - carry;
    uint8_t result8 = static_cast<uint8_t>(result & 0xFF);

    // Write result back into accumulator
    registers->setA(result8);

    // Set flags according to GameBoy CPU SBC rules:
    registers->setFlagZ(result8 == 0);   // Zero flag if result is zero
    registers->setFlagN(true);            // Subtract flag set
    // Half-carry flag set if borrow from bit 4 occurs
    registers->setFlagH(((A & 0xF) - (value & 0xF) - carry) < 0);
    // Carry flag set if borrow (result < 0)
    registers->setFlagC(result < 0);

    // Update cycles for immediate SBC, typically 8 cycles
    cycles += 8;

    // Optional ML logging:
    // Inputs: A before, immediate value, carry flag before
    // Outputs: result A, flags Z, N, H, C
}

void CPU::INC_r(Reg8 r) {
    // Fetch the value of the register to increment
    uint8_t val = 0;
    switch (r) {
        case Reg8::A: val = registers->getA(); break;
        case Reg8::B: val = registers->getB(); break;
        case Reg8::C: val = registers->getC(); break;
        case Reg8::D: val = registers->getD(); break;
        case Reg8::E: val = registers->getE(); break;
        case Reg8::H: val = registers->getH(); break;
        case Reg8::L: val = registers->getL(); break;
        default: val = 0; break; // Defensive fallback
    }

    uint8_t result = val + 1;

    // Update flags as per GameBoy CPU spec for INC r:
    // Z - Set if result == 0
    // N - Reset
    // H - Set if carry from bit 3
    // C - Not affected

    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    // Half carry if (val & 0xF) + 1 > 0xF
    registers->setFlagH(((val & 0xF) + 1) > 0xF);
    // Carry flag unaffected, so no set here

    // Write back to the register
    switch (r) {
        case Reg8::A: registers->setA(result); break;
        case Reg8::B: registers->setB(result); break;
        case Reg8::C: registers->setC(result); break;
        case Reg8::D: registers->setD(result); break;
        case Reg8::E: registers->setE(result); break;
        case Reg8::H: registers->setH(result); break;
        case Reg8::L: registers->setL(result); break;
        default: break; // Defensive fallback
    }

    // Optional ML logging hook:
    // Log input val, result, updated flags Z,N,H,C, and affected register
}

void CPU::INC_pHL() {
    uint16_t addr = registers->getHL();
    uint8_t val = memory->readByte(addr);
    uint8_t result = val + 1;

    // Set flags according to GameBoy CPU INC (HL) instruction:
    // Z - Set if result == 0
    // N - Reset (false)
    // H - Set if carry from bit 3 to bit 4
    // C - Not affected

    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(((val & 0xF) + 1) > 0xF);
    // Carry flag unchanged, so no set/reset here

    // Write result back to memory at address HL
    memory->writeByte(addr, result);

    // ML Logging possibility:
    // Inputs: original value at (HL)
    // Outputs: new value at (HL), affected flags Z, N, H
}

void CPU::DEC_r(Reg8 r) {
    uint8_t val = 0;
    // Fetch value of the register specified by enum
    switch (r) {
        case Reg8::A: val = registers->getA(); break;
        case Reg8::B: val = registers->getB(); break;
        case Reg8::C: val = registers->getC(); break;
        case Reg8::D: val = registers->getD(); break;
        case Reg8::E: val = registers->getE(); break;
        case Reg8::H: val = registers->getH(); break;
        case Reg8::L: val = registers->getL(); break;
        default: val = 0; break;  // Defensive fallback
    }

    uint8_t result = val - 1;

    // Set flags according to GameBoy CPU DEC r specification:
    // Z - Set if result == 0
    // N - Set (because DEC is subtract)
    // H - Set if borrow from bit 4 (i.e., if (val & 0xF) == 0)
    // C - Not affected

    registers->setFlagZ(result == 0);
    registers->setFlagN(true);
    registers->setFlagH((val & 0xF) == 0);
    // Carry flag unchanged

    // Write back the decremented value to the appropriate register
    switch (r) {
        case Reg8::A: registers->setA(result); break;
        case Reg8::B: registers->setB(result); break;
        case Reg8::C: registers->setC(result); break;
        case Reg8::D: registers->setD(result); break;
        case Reg8::E: registers->setE(result); break;
        case Reg8::H: registers->setH(result); break;
        case Reg8::L: registers->setL(result); break;
        default: break;
    }

    // ML logging suggestions:
    // - Input: original register value, output: decremented value, flags Z, N, H, C
}

void CPU::DEC_pHL() {
    uint16_t addr = registers->getHL();
    uint8_t val = memory->readByte(addr);
    uint8_t result = val - 1;

    // Flags update according to GameBoy CPU DEC (HL) instruction:
    // Z - Set if result == 0
    // N - Set (because DEC is subtraction)
    // H - Set if borrow from bit 4 (half borrow)
    // C - Not affected

    registers->setFlagZ(result == 0);
    registers->setFlagN(true);
    // Half borrow occurs if low nibble of val is zero before decrement
    registers->setFlagH((val & 0xF) == 0);
    // Carry flag remains unchanged

    memory->writeByte(addr, result);

    // ML logging hooks:
    // Inputs: original value at (HL)
    // Outputs: decremented value at (HL), flags Z, N, H, C
}

void CPU::ADC_r8() {
    uint8_t value = fetch();  // Fetch immediate 8-bit operand

    uint8_t A = registers->getA();
    uint8_t carry = registers->getFlagC() ? 1 : 0;

    // Perform ADC operation: A + value + carry
    uint16_t result = A + value + carry;

    // Set flags according to GameBoy CPU rules
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(false);
    registers->setFlagH(((A & 0xF) + (value & 0xF) + carry) > 0xF);
    registers->setFlagC(result > 0xFF);

    // Write result back to A register
    registers->setA(static_cast<uint8_t>(result & 0xFF));

    // Optional ML logging:
    // Inputs: A before, immediate value, carry flag
    // Outputs: result A, updated flags (Z,N,H,C)
}

// Logical Instructions: Categories 54-61

void CPU::AND_r(Reg8 r) {
    uint8_t val = 0;
    switch (r) {
        case Reg8::A: val = registers->getA(); break;
        case Reg8::B: val = registers->getB(); break;
        case Reg8::C: val = registers->getC(); break;
        case Reg8::D: val = registers->getD(); break;
        case Reg8::E: val = registers->getE(); break;
        case Reg8::H: val = registers->getH(); break;
        case Reg8::L: val = registers->getL(); break;
        default: val = 0; break; // Defensive fallback
    }

    uint8_t A = registers->getA();
    uint8_t result = A & val;

    registers->setA(result);

    // Flags update according to GameBoy CPU AND rules:
    // Z - Set if result is zero
    // N - Reset (false)
    // H - Set (true)
    // C - Reset (false)
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(true);
    registers->setFlagC(false);

    // Optional ML logging:
    // Inputs: A before, val sourced
    // Output: result in A and updated flags
}

void CPU::AND_pHL() {
    uint16_t addr = registers->getHL();
    uint8_t value = memory->readByte(addr);
    uint8_t A = registers->getA();

    uint8_t result = A & value;
    registers->setA(result);

    // Flags update according to GameBoy CPU AND rules:
    // Z - Set if result == 0
    // N - Reset
    // H - Set
    // C - Reset
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(true);
    registers->setFlagC(false);

    // ML logging can record:
    // - Inputs: A before, value at (HL)
    // - Outputs: A result, flags Z,N,H,C
}

void CPU::AND_d8() {
    uint8_t value = fetch();  // Fetch immediate 8-bit operand
    
    uint8_t A = registers->getA();
    uint8_t result = A & value;

    registers->setA(result);

    // Flags update according to GameBoy CPU AND rules:
    // Z - Set if result == 0
    // N - Reset
    // H - Set
    // C - Reset
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(true);
    registers->setFlagC(false);

    // Optional ML logging:
    // Inputs: A before, immediate value
    // Outputs: A after operation, flags Z, N, H, C
}

void CPU::OR_r(Reg8 r) {
    uint8_t val = 0;
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

    uint8_t A = registers->getA();
    uint8_t result = A | val;
    registers->setA(result);

    // Update flags according to GameBoy CPU OR instruction:
    // Z - Set if result == 0
    // N - Reset
    // H - Reset
    // C - Reset
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(false);

    // ML logging:
    // Inputs: A before, val
    // Outputs: A result, flags Z,N,H,C
}

void CPU::OR_pHL() {
    uint16_t addr = registers->getHL();
    uint8_t value = memory->readByte(addr);
    uint8_t A = registers->getA();

    uint8_t result = A | value;
    registers->setA(result);

    // Flag updates per GameBoy OR spec:
    // Z - Set if result == 0
    // N - Reset
    // H - Reset
    // C - Reset
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(false);

    // ML logging points:
    // inputs: A before, memory value at HL
    // outputs: result A, flags Z, N, H, C
}

void CPU::OR_d8() {
    uint8_t value = fetch();  // Fetch immediate 8-bit operand

    uint8_t A = registers->getA();
    uint8_t result = A | value;

    registers->setA(result);

    // Update flags according to GameBoy CPU OR instruction:
    // Z - Set if result == 0
    // N - Reset
    // H - Reset
    // C - Reset
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(false);

    // Optional ML logging:
    // Inputs: A before, immediate value
    // Outputs: A result, flags Z, N, H, C
}

void CPU::XOR_r(Reg8 r) {
    uint8_t val = 0;
    switch (r) {
        case Reg8::A: val = registers->getA(); break;
        case Reg8::B: val = registers->getB(); break;
        case Reg8::C: val = registers->getC(); break;
        case Reg8::D: val = registers->getD(); break;
        case Reg8::E: val = registers->getE(); break;
        case Reg8::H: val = registers->getH(); break;
        case Reg8::L: val = registers->getL(); break;
        default: val = 0; break; // Defensive fallback
    }

    uint8_t A = registers->getA();
    uint8_t result = A ^ val;
    registers->setA(result);

    // Flags update according to GameBoy CPU XOR spec:
    // Z - Set if result == 0
    // N - Reset
    // H - Reset
    // C - Reset
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(false);

    // ML logging:
    // Inputs: A before, val
    // Outputs: A result, flags Z,N,H,C
}

void CPU::XOR_pHL() {
    uint16_t addr = registers->getHL();
    uint8_t value = memory->readByte(addr);
    uint8_t A = registers->getA();

    uint8_t result = A ^ value;
    registers->setA(result);

    // Flags update according to GameBoy CPU XOR instruction spec:
    // Z - Set if result == 0
    // N - Reset
    // H - Reset
    // C - Reset
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(false);

    // ML logging:
    // Inputs: A before, memory value at HL
    // Outputs: A result and flags Z, N, H, C
}

void CPU::XOR_d8() {
    uint8_t value = fetch();  // Fetch immediate 8-bit operand

    uint8_t A = registers->getA();
    uint8_t result = A ^ value;

    registers->setA(result);

    // Flags update according to GameBoy CPU XOR spec:
    // Z - Set if result == 0
    // N - Reset (false)
    // H - Reset (false)
    // C - Reset (false)
    registers->setFlagZ(result == 0);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(false);

    // Optional ML logging:
    // Inputs: A before, immediate value
    // Outputs: A result, flags Z, N, H, C
}

void CPU::CP_r(Reg8 r) {
    uint8_t value = 0;
    switch (r) {
        case Reg8::A: value = registers->getA(); break;
        case Reg8::B: value = registers->getB(); break;
        case Reg8::C: value = registers->getC(); break;
        case Reg8::D: value = registers->getD(); break;
        case Reg8::E: value = registers->getE(); break;
        case Reg8::H: value = registers->getH(); break;
        case Reg8::L: value = registers->getL(); break;
        default: value = 0; break; // Defensive fallback
    }

    uint8_t A = registers->getA();
    int16_t result = static_cast<int16_t>(A) - static_cast<int16_t>(value);

    // Set flags according to GameBoy CP instruction:
    // Z - Set if result == 0
    // N - Set (subtract flag)
    // H - Set if borrow from bit 4 (half carry)
    // C - Set if borrow (if A < value)
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(true);
    registers->setFlagH(((A & 0xF) < (value & 0xF)));
    registers->setFlagC(A < value);

    // Note: A register is not modified by CP, only flags update.

    // Optional ML logging:
    // Inputs: A before, r value
    // Outputs: flags Z,N,H,C changes
}

void CPU::CP_pHL() {
    uint16_t addr = registers->getHL();
    uint8_t value = memory->readByte(addr);
    uint8_t A = registers->getA();
    int16_t result = static_cast<int16_t>(A) - static_cast<int16_t>(value);

    // Set flags according to GameBoy CP instruction (memory operand):
    // Z - Set if result == 0
    // N - Set
    // H - Half borrow from bit 4
    // C - Borrow
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(true);
    registers->setFlagH(((A & 0xF) < (value & 0xF)));
    registers->setFlagC(A < value);

    // A register unchanged

    // Optional ML logging:
    // Inputs: A before, memory[HL]
    // Outputs: flags Z,N,H,C updates
}

void CPU::CP_d8() {
    uint8_t value = fetch();  // Fetch immediate 8-bit operand

    uint8_t A = registers->getA();
    int16_t result = static_cast<int16_t>(A) - static_cast<int16_t>(value);

    // Update flags according to GameBoy CP instruction semantics:
    // - Z: set if (A - value) == 0
    // - N: set (subtract flag)
    // - H: set if borrow from bit 4 occurred (half carry)
    // - C: set if borrow occurred (if A < value)
    registers->setFlagZ((result & 0xFF) == 0);
    registers->setFlagN(true);
    registers->setFlagH(((A & 0xF) < (value & 0xF)));
    registers->setFlagC(A < value);

    // Note: A register is not modified by CP, only flags are updated.

    // Optional ML logging:
    // Inputs: A before, immediate value
    // Outputs: flags Z, N, H, C updates
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
    uint8_t A = registers->getA();
    bool N = registers->getFlagN();  // Subtraction flag
    bool H = registers->getFlagH();  // Half carry flag
    bool C = registers->getFlagC();  // Carry flag

    uint8_t correction = 0;
    bool setC = false;

    if (!N) { // After an addition
        if (C || A > 0x99) {
            correction |= 0x60;
            setC = true;
        }
        if (H || (A & 0x0F) > 0x09) {
            correction |= 0x06;
        }
        A += correction;
    } else { // After a subtraction
        if (C) {
            correction |= 0x60;
            setC = true;
        }
        if (H) {
            correction |= 0x06;
        }
        A -= correction;
    }

    registers->setA(A);

    // Flags update:
    // Z - set if A is zero after adjustment
    registers->setFlagZ(A == 0);
    // N - remains unchanged
    // H - reset after DAA
    registers->setFlagH(false);
    // C - updated if correction induced carry or was previously set
    registers->setFlagC(setC);

    // ML logging:
    // Inputs: original A, flags N,H,C before DAA
    // Outputs: A after correction, flags Z, N, H, C after correction
}

void CPU::CPL() {
    uint8_t A = registers->getA();

    // CPL flips (inverts) all bits in A register (one's complement)
    uint8_t result = ~A;
    registers->setA(result);

    // Update flags according to GameBoy CPU CPL instruction:
    // N - Set (1)
    // H - Set (1)
    // Z - Not affected
    // C - Not affected
    registers->setFlagN(true);
    registers->setFlagH(true);
    // Z and C remain unchanged

    // ML logging (optional):
    // Inputs: A before
    // Outputs: A after inversion, flags N and H set
}

void CPU::CCF() {
    // CCF: Complement (invert) carry flag
    
    bool currentCarry = registers->getFlagC();
    registers->setFlagC(!currentCarry);  // Toggle carry flag
    
    registers->setFlagN(false);  // Reset N flag
    registers->setFlagH(false);  // Reset H flag
    // Z flag unaffected
    
    // Optionally add cycles counting if your cycle variable is managed here
    cycles += 4;  // Typical machine cycles count
    
    // ML logging:
    // Inputs: flags before (N, H, C)
    // Outputs: flags after (N=0, H=0, C toggled), Z unchanged
}

void CPU::SCF() {
    // SCF: Set Carry Flag
    
    // Set carry flag
    registers->setFlagC(true);

    // Reset N and H flags
    registers->setFlagN(false);
    registers->setFlagH(false);

    // Z flag remains unchanged

    // TODO: Log input flags, output flags for ML dataset if desired

    // Timing: add cycles
    //cycles += 4;  // or as per your cycle accounting scheme
}

void CPU::RLCA() {
    uint8_t A = registers->getA();

    // Extract bit 7 to set carry
    bool bit7 = (A & 0x80) != 0;

    // Rotate A left circular: bit7 moves to bit 0
    uint8_t result = (A << 1) | (bit7 ? 1 : 0);
    registers->setA(result);

    // Update flags according to GameBoy RLCA spec:
    // Z - Reset (0) — RLCA does NOT affect zero flag
    // N - Reset
    // H - Reset
    // C - Set if old bit 7 was 1
    registers->setFlagZ(false);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(bit7);

    // ML logging:
    // Inputs: A before
    // Outputs: A after, flags N, H, C (Z ignored/reset)
}

void CPU::RLA() {
    uint8_t A = registers->getA();
    bool oldCarry = registers->getFlagC();

    // Shift A left by 1, insert old carry in bit 0
    bool bit7 = (A & 0x80) != 0;
    uint8_t result = (A << 1) | (oldCarry ? 1 : 0);

    registers->setA(result);

    // Set flags per GameBoy spec:
    // Z - Reset (not affected)
    // N - Reset
    // H - Reset
    // C - Set if old bit7 was set
    registers->setFlagZ(false);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(bit7);

    // ML logging hooks:
    // Inputs: A before, carry before
    // Outputs: A after, flags N,H,C (Z reset)
}

void CPU::RRCA() {
    uint8_t A = registers->getA();

    // Extract bit 0 to set carry
    bool bit0 = (A & 0x01) != 0;

    // Rotate A right circular: bit0 moves to bit 7
    uint8_t result = (A >> 1) | (bit0 ? 0x80 : 0);
    registers->setA(result);

    // RLCA flag update rules:
    // Z - Reset (0), not affected
    // N - Reset
    // H - Reset
    // C - Set if old bit 0 was 1
    registers->setFlagZ(false);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(bit0);

    // ML Logging:
    // Inputs: A before rotation
    // Outputs: A after, flags N, H, C (Z reset)
}

void CPU::RRA() {
    uint8_t A = registers->getA();
    bool oldCarry = registers->getFlagC();

    // Extract bit 0 before shift to set carry
    bool bit0 = (A & 0x01) != 0;

    // Rotate A right through carry: old carry moves into bit 7
    uint8_t result = (A >> 1) | (oldCarry ? 0x80 : 0);
    registers->setA(result);

    // Flags per GameBoy spec:
    // Z - Reset (not affected)
    // N - Reset
    // H - Reset
    // C - Updated to old bit 0
    registers->setFlagZ(false);
    registers->setFlagN(false);
    registers->setFlagH(false);
    registers->setFlagC(bit0);

    // ML Logging:
    // Inputs: A before, carry flag before
    // Outputs: A after, flags N, H, C (Z reset)
}

void CPU::RST(uint8_t address) {
    // Push current PC onto the stack
    uint16_t pc = registers->getPC();
    uint16_t sp = registers->getSP();

    sp -= 2;
    memory->writeByte(sp, pc);
    registers->setSP(sp);

    // Jump to fixed address
    registers->setPC(address);

    // RST takes 16 cycles (4 machine cycles)
    cycles += 16;

    // ML Logging:
    // Input: PC before, SP before
    // Output: PC after, SP aft16;

    // ML Logging:
    // Input: PC before, SP before
    // Output: PC after, SP after, memory write at SP
}

//Prefix CB

void CPU::PrefixCB() {
    // Fetch the next opcode byte to select CB-prefixed instruction
    uint8_t cbOpcode = fetch();

    // Example switch for CB-prefixed instructions (expand as needed)
    switch (cbOpcode) {
        case 0x00: // RLC B
            //RLC_r(Reg8::B);
            break;
        case 0x01: // RLC C
            //RLC_r(Reg8::C);
            break;
        // ... add all other CB-prefixed instructions with proper Reg8/bit parameters

        default:
            // Handle invalid CB opcode or log error
            break;
    }

    // Update cycles according to each CB instruction specification

    // Optionally log CB prefix and instruction for ML dataset
}