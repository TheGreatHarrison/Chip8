#include "instructions.h"
#include "cpu.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define ON_COLOR 0xFFFFFFFF
#define OFF_COLOR 0xFF000000

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

void addCarry(struct cpu* cpu, uint8_t vx , uint_8_t vy)
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

