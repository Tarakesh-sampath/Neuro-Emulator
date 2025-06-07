#include "joypad.h"
#include "memory.h"
#include <SDL2/SDL.h>

Joypad::Joypad(Memory &mem)
    : memory(mem),direction_bits(0x0F), button_bits(0x0F),
      select_direction(false), select_button(false) {}
      
void Joypad::requestInterrupt() {
    // IF is at io_regs[0x0F], IE is at ie
    // Bit 4 corresponds to Joypad interrupt
    if ((memory.io_regs[0x0F] & memory.ie & 0x10) == 0x10) {
        memory.io_regs[0x0F] |= 0x10;  // Set Joypad interrupt flag
    }
}

void Joypad::keyDown(int key) {
    uint8_t prev_direction = direction_bits;
    uint8_t prev_button = button_bits;
    switch(key) {
        case SDLK_z:    button_bits &= ~(1 << 0); break; // A
        case SDLK_x:    button_bits &= ~(1 << 1); break; // B
        case SDLK_RSHIFT: button_bits &= ~(1 << 2); break; // Select
        case SDLK_RETURN: button_bits &= ~(1 << 3); break; // Start

        case SDLK_d:    direction_bits &= ~(1 << 0); break; // Right
        case SDLK_a:    direction_bits &= ~(1 << 1); break; // Left
        case SDLK_w:    direction_bits &= ~(1 << 2); break; // Up
        case SDLK_s:    direction_bits &= ~(1 << 3); break; // Down
    }

    // If previously released (bit=1) and now pressed (bit=0), request interrupt
    if ((direction_bits & ~prev_direction) != 0 || (button_bits & ~prev_button) != 0) {
        requestInterrupt();
    }
}

void Joypad::keyUp(int key) {
    switch(key) {
        case SDLK_z:    button_bits |= (1 << 0); break; // A
        case SDLK_x:    button_bits |= (1 << 1); break; // B
        case SDLK_RSHIFT: button_bits |= (1 << 2); break; // Select
        case SDLK_RETURN: button_bits |= (1 << 3); break; // Start

        case SDLK_w:    direction_bits |= (1 << 2); break; // Up
        case SDLK_s:    direction_bits |= (1 << 3); break; // Down
        case SDLK_a:    direction_bits |= (1 << 1); break; // Left
        case SDLK_d:    direction_bits |= (1 << 0); break; // Right
    }
}

void Joypad::writeJoypadRegister(uint8_t value) {
    // CPU sets which group to select by writing bits 4 & 5
    select_direction = !(value & (1 << 4)) ; // P14 is active low select
    select_button = !(value & (1 << 5));     // P15 is active low select
}

uint8_t Joypad::readJoypadRegister() const {
    // Bits 4,5 and upper bits are set as 1 (unused bits high)
    uint8_t result = 0xCF; // 11001111 default

    // Set bits 4 and 5 based on selection (0 = selected)
    result &= ~( (select_direction ? (1 << 4) : 0) | (select_button ? (1 << 5) : 0) );

    // Return lower nibble depending on selection
    if (select_direction && !select_button) {
        // Direction keys selected (P14=0)
        result = (result & 0xF0) | (direction_bits & 0x0F);
    } else if (select_button && !select_direction) {
        // Button keys selected (P15=0)
        result = (result & 0xF0) | (button_bits & 0x0F);
    } else {
        // Both or none selected (bits high)
        result = (result & 0xF0) | 0x0F;
    }

    return result;
}