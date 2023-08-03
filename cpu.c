#include "cpu.h"
#include "instructions.h"


int cpuInit(struct cpu* cpu, char* filename)
{
    FILE* rom = fopen(filename, "rb");
    if (!rom) 
    {
        fprintf(stderr, "Unable to open file '%s'!\n", filename);
        return -1;
    }
    fread(&cpu->memory[PC_START], 1, MEMORY_SIZE - PC_START, rom);
    fclose(rom);

    memcpy(cpu->memory, font, sizeof(font));
    memset(cpu->stack, 0, sizeof(cpu->stack));
    memset(cpu->v, 0, sizeof(cpu->v));
    memset(cpu->pixels, OFF_COLOR, sizeof(cpu->pixels));

    cpu->pc = PC_START;
    cpu->i = 0;
    cpu->delayTimer = 0;
    cpu->sound = 0;
    cpu->updateDisplay = 0;
    

    return 0;

}

void cpuFetch(struct cpu* cpu)
{
    cpu->opcode.instruction = cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
}

void cpuExecute(struct cpu* cpu)
{
    uint8_t vy = cpu->v[cpu->opcode.y];
    uint8_t vx = cpu->v[cpu->opcode.x];
    switch (cpu->opcode.op) // 1st nibble
    {
    case 0x0:
        switch (cpu->opcode.n) // 4th nibble
        {
        case 0x0: // clear screen
            clearDisplay(cpu);
            break;
        default:
            break;
        }
        break;
    case 0x1: // Jump to address 1NNN
        jump(cpu);
        break;
    case 0x6: // Set the register VX to the value NN
        setRegister(cpu, cpu->opcode.x, cpu->opcode.nn);
        break;
    case 0x7: // Add the value NN to VX
        setRegister(cpu, cpu->opcode.x, vx + cpu->opcode.nn);
        break;
    case 0xA:
        setRegister(cpu, cpu->opcode.address)
        break;
    default:
        break;
    }
}

void cpuCycle(struct cpu* cpu)
{
    cpuFetch(cpu);
    cpuExecute(cpu);
    // timer or something?
}
