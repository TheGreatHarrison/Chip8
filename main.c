#include "display.h"
#include "cpu.h"

#include <stdint.h>

#include "display.h"
#include <stdint.h>



int main(void) {

    // Display init
    struct display display;
    int running = 1;

    char* filename = "IBM Logo.ch8";


    // Initialize the display
    if (display_init(&display) != 0)
    {
        return 1;
    }

    // CPU int
    struct cpu cpu;
    if (cpuInit(&cpu, filename) != 0)
    {
        return 1; // cpu init failed
    }


    while (running) 
    {
        cpuCycle(&cpu);
        
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
    }

    // Clean up and exit
    display_cleanup(&display);
    return 0;
}

