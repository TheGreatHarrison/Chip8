#include "instructions.h"
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
    cpu->pc = cpu->stack[--(cpu->stackPointer)];
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
    } else 
    {
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
    cpu->v[0xF] = (cpu->v[cpu->opcode.x] & 0x80) ? 1 : 0;
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

void setDelayTimer(struct cpu* cpu, uint8_t value) // sets VX to the current value of the delay timer
{
    cpu->delayTimer = value;
}

void setSoundTimer(struct cpu* cpu, uint8_t value)
{
    cpu->soundTimer = value;
}

void addtoIndex(struct cpu* cpu, uint8_t vx)
{
    if (cpu->i+vx > 0xFFF || cpu->i+vx < 0x1000)
    {
        cpu->v[0xF] = 1; // over flow
    } else 
    {
        cpu->v[0xF] = 0;
    }

    cpu->i += vx;
}

void skip(struct cpu* cpu, uint8_t skip)
{
    if (skip)
    {
        cpu->pc += 2;
    }
}

void waitKeyPress(struct cpu* cpu)
{
    cpu->pc -= 2;

    for (int i = 0; i <= 16; i++)
    {
        if (SDL_GetKeyboardState(NULL)[key_map[i]])
        {
            cpu->v[cpu->opcode.x] = i;
            cpu += 2;
            break;
        }
    }
}

void decimalConvert(struct cpu* cpu)
{
    cpu->memory[cpu->i] = cpu->v[cpu->opcode.x] / 100;
    cpu->memory[cpu->i+1] = (cpu->v[cpu->opcode.x] / 10) % 10;
    cpu->memory[cpu->i+2] = cpu->v[cpu->opcode.x] % 10;
}

void copyToMemory(struct cpu* cpu)
{
    memcpy(cpu->memory + cpu->i, cpu->v, cpu->opcode.x + 1);
}

void copyFromMemory(struct cpu* cpu) 
{
    memcpy(cpu->v, cpu->memory + cpu->i, cpu->opcode.x + 1);
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

