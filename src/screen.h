#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "chip8.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

typedef struct Screen {
   SDL_Window* window; 
   SDL_Renderer* renderer;
   SDL_Texture* texture;
   uint8_t width, height;
} Screen;

Screen* screen_new(uint8_t, uint8_t);
void screen_free(Screen*);
void screen_events(Screen*, Chip8*);
void screen_draw(Screen*, Chip8*);
void screen_update(Screen*, Chip8*);

