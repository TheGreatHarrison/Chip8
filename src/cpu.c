#include "cpu.h"
#include "instructions.h"


int cpuInit(struct cpu* cpu, const char* filename)
{
    const char* directory = "testRoms/";
    char file[80];
    sprintf(file,"%s%s", directory, filename);

    FILE* rom = fopen(file, "rb");
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
    cpu->soundTimer = 0;
    cpu->updateDisplay = 0;
    cpu->stackPointer = 0;

    srand(time(NULL));
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
                case 0xE:
                    jumpToStack(cpu);
                    break;
            }
            break;
        case 0x1: // Jump to address 1NNN
            jump(cpu);
            break;
        case 0x2:
            call(cpu);
            break;
        case 0x3:
            skip(cpu, vx == cpu->opcode.nn); // skip one instruction if the value in VX is equal to NN,
            break;
        case 0x4:
            skip(cpu, vx != cpu->opcode.nn);
            break;
        case 0x5:
            skip(cpu, vx == vy);
            break;
        case 0x6: // Set the register VX to the value NN
            setRegister(cpu, cpu->opcode.x, cpu->opcode.nn);
            break;
        case 0x7: // Add the value NN to VX
            setRegister(cpu, cpu->opcode.x, vx + cpu->opcode.nn);
            break;
        case 0x8:
            switch(cpu->opcode.n)
            {
            case 0x0:
                assignRegister(cpu, cpu->opcode.x, vy);
                break;
            case 0x1:
                assignRegister(cpu, cpu->opcode.x, vx | vy); // OR
                break;
            case 0x2:
                assignRegister(cpu, cpu->opcode.x, vx & vy); // AND
                break;
            case 0x3:
                assignRegister(cpu, cpu->opcode.x, vx ^ vy); // XOR
                break;
            case 0x4:
                addCarry(cpu, vx, vy);
                break;
            case 0x5:
                subtractCarry(cpu, vx, vy);    //8XY5 sets VX to the result of VX - VY
                break;
            case 0x6:
                shiftRight(cpu);
                break;
            case 0x7:
                subtractCarry(cpu, vy, vx);
                break;
            case 0xE:
                shiftLeft(cpu);
                break;
            }
            break;
    case 0x9:
        skip(cpu, vx != vy);
        break;
    case 0xA:
        setIRegister(cpu, cpu->opcode.address);
        break;
    case 0xB:
        jumpAdd(cpu);
        break;
    case 0xC:
        randomNumber(cpu);
        break;
    case 0xD:
        display(cpu);
        break;
    case 0xE:
        switch (cpu->opcode.nn)
        {
            case 0x9E:
                skip(cpu, SDL_GetKeyboardState(NULL)[key_map[vx]]);
                break;
            case 0xA1:
                skip(cpu, !SDL_GetKeyboardState(NULL)[key_map[vx]]);
                break;
        }
        break;
    case 0xF:
        switch(cpu->opcode.nn)
        {
            case 0x07: // sets VX to the current value of the delay timer
                assignRegister(cpu, cpu->opcode.x, cpu->delayTimer);
                break;
            case 0x0A: // key input
                waitKeyPress(cpu);
                break;
            case 0x15: // FX15 sets the delay timer to the value in VX
                setDelayTimer(cpu, vx);
                break;
            case 0x1E:
                addtoIndex(cpu, vx);
                break;
            case 0x18: // FX18 sets the sound timer to the value in VX
                setSoundTimer(cpu, vx);
                break;
            case 0x29:
                setIRegister(cpu, vx * 5); // each sprite is 5 bytes 
                break;
            case 0x33:
                decimalConvert(cpu);
                break;
            case 0x55:
                copyToMemory(cpu);
                break;
            case 0x65:
                copyFromMemory(cpu);
                break;
        }
        break;
    }
}

void cpuUpdateTimers(struct cpu* cpu)
{
    if (cpu->delayTimer > 0)
    {
        cpu->delayTimer--;
    }

    if (cpu->soundTimer > 0)
    {
        cpu->soundTimer--;
        // Play sound
    }
}

void cpuCycle(struct cpu* cpu)
{
    cpuFetch(cpu);
    cpuExecute(cpu);
    cpuUpdateTimers(cpu);
}
