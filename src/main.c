#include <time.h>
#include "chip8.h"
#include "renderer.h"

int main(int ac, char **av)
{
    Chip8* c = chip8_new();
    Renderer* r = renderer_new(64, 32);

    srand(time(NULL));

    chip8_load_rom(c, av[1]);
    bool quit = false;
    while (!quit) {
        chip8_cycle(c);
        renderer_events(r, c, &quit);
    }
    chip8_free(c);
    return 0;
}
