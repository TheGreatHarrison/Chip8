#pragma once
#include <SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

struct display
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  TTF_Font* ttffont;

};

int display_init(struct display* display, char* displayName, int ttf);
int display_check_error(char* name, void* pointer);
void display_cleanup(struct display* display);
void display_draw(struct display* display, uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT]);
