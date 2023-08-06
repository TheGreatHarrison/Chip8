#pragma once

#include <stdint.h>
#include "cpu.h"

void clearDisplay(struct cpu* cpu);
void jump(struct cpu* cpu);
void jumpToStack(struct cpu* cpu);
void setRegister(struct cpu* cpu, uint8_t registerIndex, uint8_t value);
void setIRegister(struct cpu* cpu, uint16_t Ivalue);
void skip(struct cpu* cpu, int expression);
void call(struct cpu* cpu);
void assignRegister(struct cpu* cpu, uint8_t a, uint8_t b);
void addCarry(struct cpu* cpu, uint8_t vx , uint_8_t vy);
void display(struct cpu* cpu);

