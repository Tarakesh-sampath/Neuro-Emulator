#ifndef JOYPAD_H
#define JOYPAD_H

#include <cstdint>

class Memory;

class Joypad {
public:
    Joypad(Memory& mem);

    // Call these on respective keyboard events
    void keyDown(int sdl_keycode);
    void keyUp(int sdl_keycode);

    // Read joypad register value (0xFF00)
    uint8_t readJoypadRegister() const;

    // Set select bits (P14, P15) from CPU writes to 0xFF00
    void writeJoypadRegister(uint8_t value);

private:
    Memory& memory;
    
    // Button state bits. 0 means pressed, 1 means released (per hardware)
    uint8_t direction_bits; // Up, Down, Left, Right (bits 0..3)
    uint8_t button_bits;    // A, B, Select, Start (bits 0..3)

    // Controls which group is selected by CPU: bit 4 (P14), bit 5 (P15)
    bool select_direction;
    bool select_button;
    
    // Helper to raise Joypad interrupt flag in IF register
    void requestInterrupt();
};

#endif // JOYPAD_H