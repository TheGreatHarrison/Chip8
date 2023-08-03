#include "instructions.h"
#include "cpu.h"

void clearDisplay(struct cpu* cpu)
{
    memset(cpu->pixels, OFF_COLOR, sizeof(cpu->pixels));
    cpu->updateDisplay = 1; // NOT SURE
}

void jump(struct cpu* cpu)
{
    cpu->pc = cpu->opcode.address;
}

void setRegister(struct cpu* cpu, uint8_t registerIndex, uint8_t value)
{
    cpu->v[registerIndex] = value;
}

void setRegister(struct cpu* cpu, uint16_t Ivalue)
{
    cpu->i = Ivlaue;
}