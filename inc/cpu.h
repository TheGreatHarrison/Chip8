#pragma once

#include "opcode.h"
#include <SDL.h>
#include "constants.h"

struct cpu
{
    uint8_t memory[MEMORY_SIZE];
    uint16_t pc;
    uint16_t i;
    uint16_t stack[16];
    uint8_t stackPointer;
    uint8_t v[16];

    uint8_t delayTimer;
    uint8_t soundTimer;

    uint8_t updateDisplay;

    uint16_t instruction;
    union opcode opcode;

    uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

};

// cpu things
int cpuInit(struct cpu* cpu, const char* filename);
void cpuFetch(struct cpu* cpu);
void cpuExecute(struct cpu* cpu);
void cpuUpdateTimers(struct cpu* cpu);
void cpuCycle(struct cpu* cpu);

