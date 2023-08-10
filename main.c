#include "display.h"
#include "cpu.h"
#include <SDL2/SDL.h>

#include "display.h"
#include <stdint.h>


int main(void)
{
    // Display init
    struct display display;
    int running = 1;
    uint32_t start_tick;
    uint32_t elapsedTime;

    uint16_t cpuHz = 540;
    uint16_t cpuCycles = cpuHz / 60; // Get this configurable in future or adjust for each game
    uint16_t update60Hz = 1000 / 60; // time per a 60hz update (16.6666 ms)

    char* filename = "IBM Logo.ch8";
    char* filename2 = "Test1.ch8";
    char* filename3 = "test3.ch8";
    char* filename4 = "cavern.ch8";
    char* filename5 = "Tetris.ch8";

    // Initialize the display
    if (display_init(&display) != 0)
    {
        return 1;
    }


    // CPU int
    struct cpu cpu;
    if (cpuInit(&cpu, filename5) != 0)
    {
        return 1; // cpu init failed
    }


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
        if(elapsedTime < update60Hz)
        {
            SDL_Delay(update60Hz - elapsedTime);
        }
    }
    // Clean up and exit
    display_cleanup(&display);
    return 0;
}

