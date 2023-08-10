#pragma once

#include <stdint.h>

union opcode
{
    u_int16_t instruction;
    struct
    {
        uint8_t n : 4; // A 4-bit number.
        uint8_t y : 4; // Used to look up one of the 16 registers (VY) from V0 through VF
        uint8_t x : 4; // Used to look up one of the 16 registers (VX) from V0 through VF.
        uint8_t op : 4;
    };

    struct
    {
        uint8_t nn : 8; //  An 8-bit immediate number.
    };

    struct
    {
        uint16_t address : 12; // A 12-bit immediate memory address.
    };

};
