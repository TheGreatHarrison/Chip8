
#include "cpu.h"
#include "constants.h"
#include "display.h"

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

char* roms[ROM_COUNT] = {
    "Pong.rom",
    "Tetris.ch8",
    "Cavern.ch8",
    "SpaceInvaders.ch8"
};

int selectRom() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
        return -1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Chip-8 ROM Selection", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*SCALE/2, SCREEN_HEIGHT*SCALE/2, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError()); 
        return -1;
    }

    // Load font for text rendering
    TTF_Font* ttffont = TTF_OpenFont("inc/quadrangle.ttf", 10);
    if (!ttffont) {
        printf("Font loading failed: %s\n", TTF_GetError());
        return -1;
    }

    int selectedRom = 0;
    int quit = 0;

    // Create an off-screen texture to render the whole scene
    SDL_Texture* offScreenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH * SCALE / 2, SCREEN_HEIGHT * SCALE / 2);

    while (!quit) {
        // Handle input events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                //quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedRom = (selectedRom - 1 + ROM_COUNT) % ROM_COUNT;
                        break;
                    case SDLK_DOWN:
                        selectedRom = (selectedRom + 1) % ROM_COUNT;
                        break;
                    case SDLK_RETURN:
                        quit = 1;
                        break;
                }
            }
        }

        SDL_SetRenderTarget(renderer, offScreenTexture);
        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Color textColor;
        int textHeight = 25; // Adjust this value for appropriate spacing
        int startY = (SCREEN_HEIGHT * SCALE / 2 - (textHeight * ROM_COUNT)) / 2;

        // Render menu
        for (int i = 0; i < ROM_COUNT; i++) {
            if (i == selectedRom) {
                textColor.r = 255;
                textColor.g = 0;
                textColor.b = 0;
                textColor.a = 255;
            } else {
                textColor.r = 255;
                textColor.g = 255;
                textColor.b = 255;
                textColor.a = 255;
            }
            SDL_Surface* textSurface = TTF_RenderText_Solid(ttffont, roms[i], textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            int textWidth, textHeight;
            SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

            int posX = (SCREEN_WIDTH * SCALE / 2 - textWidth) / 2;
            int posY = startY + i * textHeight;

            SDL_Rect textRect = { posX, posY, textWidth, textHeight };

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            // Clean up texture and surface
        }

        // Reset render target to the default window
        SDL_SetRenderTarget(renderer, NULL);

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderCopy(renderer, offScreenTexture, NULL, NULL);

        SDL_RenderPresent(renderer);

        SDL_Delay(UPDATE60HZ);
    }

    TTF_CloseFont(ttffont);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return selectedRom;
}

int main(void)
{
    // Display init
    struct display display;
    uint32_t start_tick;
    uint32_t elapsedTime;

    uint16_t cpuHz = 540;
    uint16_t cpuCycles = cpuHz / 60; // Get this configurable in future or adjust for each game
    int selectedROM = selectRom();
    
    char* filename = "testRoms/IBM Logo.ch8";
    char* filename2 = "testRoms/Test1.ch8";
    char* filename3 = "testRoms/test3.ch8";
    char* filename4 = "testRoms/pong.rom";
    char* filename5 = "testRoms/Tetris.ch8";

    // Initialize the display
    if (display_init(&display) != 0)
    {
        return 1;
    }

    // CPU int
    struct cpu cpu;
    if (cpuInit(&cpu, roms[selectedROM]) != 0) // roms[selectedROM]
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

