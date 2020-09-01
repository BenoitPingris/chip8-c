#include "renderer.h"

Renderer* renderer_new(uint8_t width, uint8_t height)
{
    Renderer* r = malloc(sizeof(Renderer));

    if (!r) {
        return NULL;
    }
    r->width = width;
    r->height = height;
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        printf("Could not init SDL: %s\n", SDL_GetError());
        return NULL;
    }
    if ((r->window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * 10, height * 10, SDL_WINDOW_SHOWN)) == NULL) {
        return NULL;
    }
    r->surface = SDL_GetWindowSurface(r->window);
    return r;
}

void renderer_free(Renderer* r)
{
    SDL_DestroyWindow(r->window);
    free(r);
}

void renderer_events(Renderer* r, Chip8 *c, bool* quit)
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                *quit = true;
                printf("QUIT");
                break;
            case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    *quit = true;
                    break;
                case SDLK_x:
                {
                    c->keypad[0] = 1;
                } break;

                case SDLK_1:
                {
                    c->keypad[1] = 1;
                } break;

                case SDLK_2:
                {
                    c->keypad[2] = 1;
                } break;

                case SDLK_3:
                {
                    c->keypad[3] = 1;
                } break;

                case SDLK_q:
                {
                    c->keypad[4] = 1;
                } break;

                case SDLK_w:
                {
                    c->keypad[5] = 1;
                } break;

                case SDLK_e:
                {
                    c->keypad[6] = 1;
                } break;

                case SDLK_a:
                {
                    c->keypad[7] = 1;
                } break;

                case SDLK_s:
                {
                    c->keypad[8] = 1;
                } break;

                case SDLK_d:
                    c->keypad[9] = 1;
                    break;
                case SDLK_z:
                {
                    c->keypad[0xA] = 1;
                } break;

                case SDLK_c:
                {
                    c->keypad[0xB] = 1;
                } break;

                case SDLK_4:
                {
                    c->keypad[0xC] = 1;
                } break;

                case SDLK_r:
                {
                    c->keypad[0xD] = 1;
                } break;

                case SDLK_f:
                {
                    c->keypad[0xE] = 1;
                } break;

                case SDLK_v:
                {
                    c->keypad[0xF] = 1;
                } break;
            }
            break;
            case SDL_KEYUP:
            switch (e.key.keysym.sym) {
                case SDLK_x:
                {
                    c->keypad[0] = 0;
                } break;

                case SDLK_1:
                {
                    c->keypad[1] = 0;
                } break;

                case SDLK_2:
                {
                    c->keypad[2] = 0;
                } break;

                case SDLK_3:
                {
                    c->keypad[3] = 0;
                } break;

                case SDLK_q:
                {
                    c->keypad[4] = 0;
                } break;

                case SDLK_w:
                {
                    c->keypad[5] = 0;
                } break;

                case SDLK_e:
                {
                    c->keypad[6] = 0;
                } break;

                case SDLK_a:
                {
                    c->keypad[7] = 0;
                } break;

                case SDLK_s:
                {
                    c->keypad[8] = 0;
                } break;

                case SDLK_d:
                {
                    c->keypad[9] = 0;
                } break;

                case SDLK_z:
                {
                    c->keypad[0xA] = 0;
                } break;

                case SDLK_c:
                {
                    c->keypad[0xB] = 0;
                } break;

                case SDLK_4:
                {
                    c->keypad[0xC] = 0;
                } break;

                case SDLK_r:
                {
                    c->keypad[0xD] = 0;
                } break;

                case SDLK_f:
                {
                    c->keypad[0xE] = 0;
                } break;

                case SDLK_v:
                {
                    c->keypad[0xF] = 0;
                } break;
            }
            break;
        }
    }
}
