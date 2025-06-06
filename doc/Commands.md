#### Compile the Emulator
g++ -std=c++17 -I./emulator/include ./emulator/*.cpp -o emulator_test
./emulator_test ./Tetris.gb ./emulator/opcode.json
