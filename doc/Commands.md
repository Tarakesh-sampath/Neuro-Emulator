#### Compile the Emulator
g++ -std=c++17 -I./emulator/include ./emulator/*.cpp -lSDL2 -o emulator_test
./emulator_test ./Tetris.gb ./emulator/opcodes.json
