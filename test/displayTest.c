#include <stdint.h>

#include "display.h"
#include "cpu.h"
#include "constants.h"

// Function to create a test pattern
void createTestPattern(uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT]) 
{
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            // Generate a gradient pattern using RGB values
            uint8_t r = (x * 255) / SCREEN_WIDTH;
            uint8_t g = (y * 255) / SCREEN_HEIGHT;
            uint8_t b = 255 - ((x * 255) / SCREEN_WIDTH);

            // Combine RGB values into a 32-bit color
            pixels[y * SCREEN_WIDTH + x] = (255 << 24) | (r << 16) | (g << 8) | b;
        }
    }
}

int main(void) 
{
    struct display display;
    uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    int running = 1;

    // Initialize the display
    if (display_init(&display) != 0) 
    {
        return 1;
    }

    // Create a test pattern in the pixels array
    createTestPattern(pixels);

    // Main loop
    while (running) 
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = 0; // Set running to false to exit the loop on window close
            }
        }

        // Draw the test pattern on the screen
        display_draw(&display, pixels);

        // Wait for a short while (you can replace this with your game loop)
        SDL_Delay(16);
    }

    // Clean up and exit
    display_cleanup(&display);
    return 0;
}

