#include "lib/cpu.h"


int cpu_init(struct cpu* cpu, char* filename)
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

    cpu->pc = PC_START;
    cpu->i = 0;
    cpu->delay_timer = 0;
    cpu->sound = 0;

    return 0;

}

void cpu_fetch(struct cpu* cpu)
{
    uint16_t instruction = cpu->memory[cpu->pc] << 8 | cpu->memory[cpu->pc + 1];;
    cpu->pc += 2;
}
void cpu_execute(struct cpu* cpu)
{
    // do all op codes here
}

void cpu_cycle(struct cpu* cpu)
{
    cpu_fetch(cpu);
    cpu_execute(cpu);
    // timer or something?
}
