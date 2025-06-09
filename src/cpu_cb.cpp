#include "cpu_cb.h"
#include "cpu.h"

void initCBOpTable(OpHandler* cbTable, CPU* cpu) {
    for (int i = 0; i < 0x100; ++i) cbTable[i] = &CPU::op_UNIMPLEMENTED;
}