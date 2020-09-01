#include <time.h>
#include <unistd.h>
#include "chip8.h"
#include "screen.h"

int main(int ac, char **av)
{
    Chip8* c = chip8_new();
    Screen* s = screen_new(64, 32);

    srand(time(NULL));

    chip8_load_rom(c, av[1]);
    while (c->running) {
        chip8_cycle(c);
        screen_update(s, c);
        usleep(500);
    }
    chip8_free(c);
    return 0;
}
