# 📘 GAME BOY EMULATOR: CORE COMPONENTS

### 🧠 1. **CPU Emulation (Sharp LR35902)**

* Instruction decoder & dispatcher (opcodes 0x00 to 0xFF and 0xCB-prefixed)
* Registers: A, F, B, C, D, E, H, L, SP, PC
* Flags: Z (zero), N (subtract), H (half carry), C (carry)
* Arithmetic & logic operations
* Control flow: jumps, calls, returns, conditional branches
* Timing & instruction cycles
* instruction set refference [Here](https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html)

### 📦 2. **Memory Map**

* ROM (up to 32KB without banking)
* RAM (8KB work RAM, 8KB video RAM, 127 bytes of stack, OAM)
* Memory-mapped I/O
* Memory banking (MBC1, MBC2 — if needed)
* Boot ROM and disabling sequence

### 🎮 3. **Input Handling**

* Joypad register (0xFF00): A, B, Start, Select, D-Pad
* Interrupt generation on keypress

### 🖼️ 4. **Graphics (PPU – Pixel Processing Unit)**

* Background tilemap & tileset rendering
* Sprites: size, priority, attributes
* Scrolling (SCX, SCY), window layer (WX, WY)
* LCD control (LCDC), LCD status (STAT)
* Line drawing & timing per scanline
* VBlank & LCD interrupts

### 🔊 5. **Audio (APU – Audio Processing Unit)** *(optional at first)*

* 4 channels (2 square wave, 1 wave, 1 noise)
* Envelope, sweep, length counters
* Sound control registers

### ⏱️ 6. **Timers and Interrupts**

* DIV, TIMA, TMA, TAC registers
* Interrupt flags (IF) and enable register (IE)
* Interrupt handling routine jump table

### 🔁 7. **Execution Loop**

* Clock management and cycles per instruction
* Frame timing (59.7 FPS)
* Sync CPU, PPU, timers

### 📂 8. **ROM Loader**

* Load `.gb` files
* Parse ROM header, extract metadata
* Optional MBC logic

### 🪛 9. **Debug Tools (Optional but Useful)**

* Memory viewer
* Instruction logger
* Step-through debugger
* Frame renderer visualization

---
test
