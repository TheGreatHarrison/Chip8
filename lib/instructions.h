#pragma once

#include <stdint.h>
#include "cpu.h"

void clearDisplay(struct cpu* cpu);
void jump(cpu);
void setRegister(struct cpu* cpu, uint8_t registerIndex, uint8_t value);
void setRegister(struct cpu* cpu, uint16_t Ivalue);