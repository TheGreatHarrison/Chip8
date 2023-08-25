#include "constants.h"
#include "display.h"
#include "romSelection.h"

#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

int selectRom(void) {
    struct display ROMdisplay;
    display_init(&ROMdisplay, "Chip-8 ROM Selection", 1);

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
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
    int textHeight = 25; // Adjust this value for appropriate spacing
    const char* titleText = "CHIP-8";

    // Create an off-screen texture to render the whole scene
    SDL_Texture* offScreenTexture = SDL_CreateTexture(ROMdisplay.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH * SCALE / 2, SCREEN_HEIGHT * SCALE / 2);

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

        SDL_SetRenderTarget(ROMdisplay.renderer, offScreenTexture);
        // Clear the renderer
        SDL_SetRenderDrawColor(ROMdisplay.renderer, 0, 0, 0, 255);
        SDL_Color textColor;
        textColor.r = 0;
        textColor.g = 255;
        textColor.b = 0;
        textColor.a = 255;
        // title
        int startY = (SCREEN_HEIGHT * SCALE / 2 - (textHeight * ROM_COUNT)) / 2;
        SDL_Surface* textSurface = TTF_RenderText_Solid(ttffont, titleText, textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ROMdisplay.renderer, textSurface);
        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

        int posX = (SCREEN_WIDTH * SCALE / 2 - textWidth) / 2;
        int posY = startY + textHeight;

        SDL_Rect textRect = { posX, posY, textWidth, textHeight };

        SDL_RenderCopy(ROMdisplay.renderer, textTexture, NULL, &textRect);
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
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ROMdisplay.renderer, textSurface);

            int textWidth, textHeight;
            SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

            int posX = (SCREEN_WIDTH * SCALE / 2 - textWidth) / 2;
            int posY = startY + (i+2) * textHeight;

            SDL_Rect textRect = { posX, posY, textWidth, textHeight};

            SDL_RenderCopy(ROMdisplay.renderer, textTexture, NULL, &textRect);
            // Clean up texture and surface
        }

        // Reset render target to the default window
        SDL_SetRenderTarget(ROMdisplay.renderer, NULL);

        // Clear the renderer
        SDL_SetRenderDrawColor(ROMdisplay.renderer, 0, 0, 0, 255);
        SDL_RenderCopy(ROMdisplay.renderer, offScreenTexture, NULL, NULL);

        SDL_RenderPresent(ROMdisplay.renderer);

        SDL_Delay(UPDATE60HZ);
    }

    TTF_CloseFont(ttffont);
    TTF_Quit();
    SDL_DestroyRenderer(ROMdisplay.renderer);
    SDL_DestroyWindow(ROMdisplay.window);
    SDL_Quit();
    return selectedRom;
}
