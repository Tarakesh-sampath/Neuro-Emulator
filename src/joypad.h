// joypad.h
#pragma once
#include <cstdint>
#include <SDL2/SDL.h>
#include <fstream>

class Joypad {
public:
    Joypad();
    ~Joypad();

    // Write to P1 register (0xFF00) - select which buttons to read
    void write(uint8_t value);

    // Read from P1 register (0xFF00)
    uint8_t read() const;

    // Update state from SDL2 keyboard events
    void handleEvent(const SDL_Event& event);

    // Log current button state to output stream (for ML training)
    void logState(std::ofstream& logFile, uint64_t frameNumber) const;

private:
    // Selected button group (bits 4 and 5 in P1)
    // bit4=0 selects direction keys, bit5=0 selects button keys
    uint8_t selectBits;

    // Current physical button states (active low)
    // Bit 0 Right, 1 Left, 2 Up, 3 Down, 4 A, 5 B, 6 Select, 7 Start
    uint8_t buttonStates;

    // Map SDL keycode to Joypad button bit
    void setButtonState(SDL_Keycode key, bool pressed);
};