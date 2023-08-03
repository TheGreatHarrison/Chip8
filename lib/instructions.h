#pragma once

#include <stdint.h>
#include "cpu.h"

void clearDisplay(struct cpu* cpu);
void jump(struct cpu* cpu);
void setRegister(struct cpu* cpu, uint8_t registerIndex, uint8_t value);
void setIRegister(struct cpu* cpu, uint16_t Ivalue);
void display(struct cpu* cpu);

