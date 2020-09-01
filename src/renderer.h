#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "chip8.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

typedef struct Renderer {
   SDL_Surface* surface; 
   SDL_Window* window; 
   uint8_t width, height;
} Renderer;

Renderer* renderer_new(uint8_t, uint8_t);
void renderer_free(Renderer*);
void renderer_events(Renderer*, Chip8*, bool*);

