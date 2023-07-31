#include "lib/display.h"
#include "lib/cpu.h"

#include <stdint.h>

#include "lib/display.h"
#include <stdint.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


int main(void) {

    // Display init
    struct display display;
    uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    int running = 1;

    // Initialize the display
    if (display_init(&display) != 0) {
        return 1;
    }

    // CPU int
    struct cpu cpu;


    while (running) 
    {

        cpu_cycle(&cpu);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0; // Set running to false to exit the loop on window close
            }
        }

        display_draw(&display, pixels);
    }

    // Clean up and exit
    display_cleanup(&display);
    return 0;
}

