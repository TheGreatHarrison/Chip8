#include "instructions.h"
#include "cpu.h"

void clearDisplay(struct cpu* cpu)
{
    memset(cpu->pixels, OFF_COLOR, sizeof(cpu->pixels));
    cpu->updateDisplay = 1; // NOT SURE
}
