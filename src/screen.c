#include "screen.h"

Screen* screen_new(uint8_t width, uint8_t height)
{
    Screen* s = malloc(sizeof(Screen));

    if (!s) {
        return NULL;
    }
    s->width = width;
    s->height = height;
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        printf("Could not init SDL: %s\n", SDL_GetError());
        return NULL;
    }
    if ((s->window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN)) == NULL
        || (s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED)) == NULL
        || (s->texture = SDL_CreateTexture(s->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 10, 10)) == NULL) {
        return NULL;
    }
    return s;
}

void screen_free(Screen* s)
{
    SDL_DestroyTexture(s->texture);
    SDL_DestroyRenderer(s->renderer);
    SDL_DestroyWindow(s->window);
    free(s);
    SDL_Quit();
}

void screen_update(Screen* s, Chip8* c)
{
    screen_draw(s, c);
    screen_events(s, c);
}

void screen_draw(Screen* s, Chip8* c)
{
    SDL_UpdateTexture(s->texture, NULL, c->screen, sizeof(uint32_t) * VIDEO_WIDTH);
    SDL_RenderClear(s->renderer);
    SDL_RenderCopy(s->renderer, s->texture, NULL, NULL);
    SDL_RenderPresent(s->renderer);
}

void screen_events(Screen* s, Chip8 *c)
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                c->running = false;
                printf("QUIT");
                break;
            case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    c->running = false;
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
