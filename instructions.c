#include "instructions.h"
#include "cpu.h"
#include "constants.h"

void clearDisplay(struct cpu* cpu)
{
    memset(cpu->pixels, OFF_COLOR, sizeof(cpu->pixels));
    //cpu->updateDisplay = 1; // NOT SURE
}

void jump(struct cpu* cpu)
{
    cpu->pc = cpu->opcode.address;
}

void jumpToStack(struct cpu* cpu)
{
    cpu->pc = cpu->stack[--cpu->stackPointer-1];
}

void setRegister(struct cpu* cpu, uint8_t registerIndex, uint8_t value)
{
    cpu->v[registerIndex] = value;
}

void setIRegister(struct cpu* cpu, uint16_t Ivalue)
{
    cpu->i = Ivalue;
}

void call(struct cpu* cpu)
{
    cpu->stack[cpu->stackPointer++] = cpu->pc; // Todo
    jump(cpu);
}

void skip(struct cpu* cpu, int expression) // not sure
{
    if (expression)
    {
        cpu->pc+=2;
    }
}

void assignRegister(struct cpu* cpu, uint8_t id, uint8_t value)
{
    cpu->v[id] = value;
}

void addCarry(struct cpu* cpu, uint8_t vx , uint8_t vy)
{
    if ( vx + vy > 255)
    {
        cpu->v[0xF] = 1;
    } else 
    {
        cpu->v[0xF] = 0;
    }
    cpu->v[cpu->opcode.x] = vx + vy;
}

void subtractCarry(struct cpu* cpu, uint8_t vx, uint8_t vy)
{
    if ( vx > vy )
    {
        cpu->v[0xF] = 1;
    } else {
        cpu->v[0xF] = 0;
    }

    cpu->v[cpu->opcode.x] = vx - vy;
}

void shiftRight(struct cpu* cpu)
{
    if  (SHIFTVXTOVY) // Set VX to the value of VY
    {
        setRegister(cpu, cpu->opcode.x, cpu->v[cpu->opcode.y]);
    }
    cpu->v[0xF] = cpu->v[cpu->opcode.x] & 1;
    cpu->v[cpu->opcode.x] >>= 1;
}

void shiftLeft(struct cpu* cpu)
{
    if  (SHIFTVXTOVY) // Set VX to the value of VY
    {
        setRegister(cpu, cpu->opcode.x, cpu->v[cpu->opcode.y]);
    }
    cpu->v[0xF] = (cpu->v[cpu->opcode.x] & 0x80) ? 1 : 0;;
    cpu->v[cpu->opcode.x] <<= 1;
}

void jumpAdd(struct cpu* cpu)
{
    // todo handle weird case
    cpu->pc = cpu->opcode.address + cpu->v[0];
}

void randomNumber(struct cpu* cpu)
{
    cpu->v[cpu->opcode.x] = (rand() % 256) & cpu->opcode.nn;
}

void display( struct cpu* cpu) // DXYN
{
    uint8_t vx = cpu->v[cpu->opcode.x];
    uint8_t vy = cpu->v[cpu->opcode.y];
    cpu->v[0xF] = 0;

    uint8_t height = cpu->opcode.n;

    for (int y = 0 ; y < height ; y++)
    {
        uint8_t sprite = cpu->memory[cpu->i + y];
        uint32_t row = (vy + y) % SCREEN_HEIGHT; 
        for (int q = 0; q < 8 ; q++ )
        {
            if ((sprite & (0x80) >> q))
            {
                uint32_t col = (vx + q) % SCREEN_WIDTH;
                uint32_t offset =  row * SCREEN_WIDTH + col;  // 2d array to 1d
                if (cpu->pixels[offset] == ON_COLOR)
                {
                    cpu->pixels[offset] = OFF_COLOR;
                    cpu->v[0xF] = 1;
                } else 
                {
                    cpu->pixels[offset] = ON_COLOR;
                }
                cpu->updateDisplay= 1;
            }
        }
    }
}

