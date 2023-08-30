
#include "cpu.h"
#include "constants.h"
#include "display.h"
#include "romSelection.h"

#include <SDL.h>
#include <stdint.h>


int main(void)
{
    // Display init
    struct display display;
    uint32_t start_tick;
    uint32_t elapsedTime;

    uint16_t cpuCycles = CPUHZ / 60; // Get this configurable in future or adjust for each game
    int selectedROM = selectRom();

    // Initialize the display
    if (display_init(&display, "Chip-8", 0) != 0)
    {
        return 1;
    }

    // CPU int
    struct cpu cpu;
    if (cpuInit(&cpu, (roms[selectedROM])) != 0) // roms[selectedROM]
    {
        return 1; // cpu init failed
    }

    int running = 1;

    while (running) 
    {
        uint32_t start_tick = SDL_GetTicks();
        for (int i=0; i < cpuCycles; i++)
        {
            cpuCycle(&cpu);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0; // Set running to false to exit the loop on window close
            }
        }

        if (cpu.updateDisplay)
        {
            display_draw(&display, cpu.pixels);
            cpu.updateDisplay = 0;
        }
        // Timers
        elapsedTime = SDL_GetTicks() - start_tick;
        if(elapsedTime < UPDATE60HZ)
        {
            SDL_Delay(UPDATE60HZ - elapsedTime);
        }
    }
    // Clean up and exit
    display_cleanup(&display);
    return 0;
}

