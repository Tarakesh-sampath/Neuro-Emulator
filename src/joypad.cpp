// joypad.cpp
#include "joypad.h"
#include <iostream>
#include <iomanip>

Joypad::Joypad()
    : selectBits(0x30)  // Default: bits 4 and 5 set (no button group selected), bits 6-7 unused, upper bits 0
    , buttonStates(0xFF) // All buttons unpressed = 1 (active low)
{}

Joypad::~Joypad() {}

void Joypad::write(uint8_t value) {
    // Only bits 4 and 5 writable in P1
    selectBits = value & 0x30;
}

uint8_t Joypad::read() const {
    /*
    P1 register bits:
    Bit 7-6: unused, always 1
    Bit 5: Select Button Keys      (0=Select, Start, B, A)
    Bit 4: Select Direction Keys   (0=Right, Left, Up, Down)
    Bit 3-0: Button states (active low, 0=pressed)

    Return bits:
    Bits 7 and 6: 1
    Bits 5 and 4: selectBits (from last write)
    Bits 3-0: depends on selectBits and actual buttons pressed
    */
    uint8_t res = 0xC0 | selectBits;

    // active low bits
    uint8_t states = 0x0F; // all released by default

    if (!(selectBits & 0x10)) { // bit4 == 0 -> direction keys
        // map direction bits 0-3
        // Joypad bits: Right(0), Left(1), Up(2), Down(3)
        states &= (buttonStates >> 0) & 0x0F;
    }
    if (!(selectBits & 0x20)) { // bit5 == 0 -> button keys
        // button keys in bits 4-7 mapped to bits 0-3 here
        // Select(6), Start(7), B(5), A(4) -> remapped as bits 0-3
        // Our buttonStates layout to 0-3 bits mapping:
        //  bit 0 = Right
        //  bit 1 = Left
        //  bit 2 = Up
        //  bit 3 = Down
        //  bit 4 = A
        //  bit 5 = B
        //  bit 6 = Select
        //  bit 7 = Start 

        uint8_t btnKeys =
            ((buttonStates & (1 << 6)) >> 6) |  // Select -> bit0
            (((buttonStates & (1 << 7)) >> 7) << 1) | // Start -> bit1
            (((buttonStates & (1 << 5)) >> 5) << 2) | // B -> bit2
            (((buttonStates & (1 << 4)) >> 4) << 3);  // A -> bit3

        states &= btnKeys;
    }

    res |= (states & 0x0F);

    return res;
}

void Joypad::handleEvent(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
        return;

    bool pressed = (event.type == SDL_KEYDOWN);
    SDL_Keycode key = event.key.keysym.sym;
    setButtonState(key, pressed);
}

void Joypad::setButtonState(SDL_Keycode key, bool pressed) {
    uint8_t mask = 0;

    // Map keys to bits
    switch (key) {
        case SDLK_d: mask = 1 << 0; break; // Right
        case SDLK_a: mask = 1 << 1; break; // Left
        case SDLK_w: mask = 1 << 2; break; // Up
        case SDLK_s: mask = 1 << 3; break; // Down
        case SDLK_k: mask = 1 << 4; break; // A
        case SDLK_l: mask = 1 << 5; break; // B
        case SDLK_RETURN: mask = 1 << 6; break; // Select
        case SDLK_SPACE: mask = 1 << 7; break;  // Start
        default: return;
    }

    if (pressed)
        buttonStates &= ~mask; // active low pressed = 0
    else
        buttonStates |= mask;  // released = 1
}

void Joypad::logState(std::ofstream& logFile, uint64_t frameNumber) const {
    // Log frame number and raw button state byte (active low)
    // For ML: ideally save as vector of 8 binary flags 0=not pressed,1=pressed
    // Here we invert active low to 0=released,1=pressed
    uint8_t pressedBits = ~buttonStates;
    logFile << frameNumber << " ";
    for (int i = 0; i < 8; ++i) {
        logFile << ((pressedBits >> i) & 1);
        if (i < 7) logFile << " ";
    }
    logFile << "\n";
}