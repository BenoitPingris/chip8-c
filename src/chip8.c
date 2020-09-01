#include "chip8.h"

Chip8* chip8_new()
{
    Chip8* c = malloc(sizeof(Chip8));
    if (!c) {
        return NULL;
    }
    memset(c, 0, sizeof(Chip8));
    c->running = true;
    c->draw_flag  = true;
    c->pc = PROGRAM_START_ADDR;  
    for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
		c->memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}
    return c;
}

void chip8_load_rom(Chip8* c, char* filename)
{
    FILE* fp = fopen(filename, "rb");
    long rom_length;
    uint8_t *rom_buffer;

    if (!fp) {
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0, SEEK_END);
    rom_length = ftell(fp);
    rewind(fp);
    rom_buffer = malloc(sizeof(uint8_t) * rom_length);
    if (!rom_buffer) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    fread(rom_buffer, sizeof(uint8_t), rom_length, fp);

    for (size_t i = 0; i < rom_length; i++) {
        c->memory[PROGRAM_START_ADDR + i] = rom_buffer[i];
    }
    free(rom_buffer);
    fclose(fp);
}

void chip8_free(Chip8* c)
{
    free(c);
}

uint16_t chip8_fetch_opcode(Chip8* c)
{
    uint16_t opcode;
    uint8_t msB = c->memory[c->pc];
    uint8_t lsB = c->memory[c->pc + 1];

    opcode = msB << 8 | lsB;
    return opcode;
}

void chip8_cycle(Chip8* c)
{
    c->opcode = chip8_fetch_opcode(c);
    c->pc += 2;
    printf("opcode 0x%X\n", c->opcode);

    chip8_execute_instructions(c);
   
    if (c->delay_timer > 0) {
        c->delay_timer--;
    }
    if (c->sound_timer > 0) {
        c->sound_timer--;
    }
}

void chip8_execute_instructions(Chip8* c)
{
    switch (c->opcode & 0xF000) {
        case 0x0000:
            switch (c->opcode & 0x00FF) {
                case 0x00E0:
                    chip8_cls(c); 
                    break;
                case 0x00EE:
                    chip8_ret(c);
                    break;
                default:
                    printf("ERROR: Unrecognized opcode 0x%X\n", c->opcode);
                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 0x1000:
            chip8_jmp(c);
            break;
        case 0x2000:
            chip8_call(c);
            break;
        case 0x3000:
            chip8_se_vx(c);
            break;
        case 0x4000:
            chip8_sne_vx(c);
            break;
        case 0x5000:
            chip8_se_vx_vy(c);
            break;
        case 0x6000:
            chip8_ld_vx(c);
            break;
        case 0x7000:
            chip8_add_vx(c);
            break;
        case 0x8000:
            switch(c->opcode & 0x000F) {
                case 0x0000:
                    chip8_ld_vx(c);
                    break;
                case 0x0001:
                    chip8_or_vx_vy(c); 
                    break;
                case 0x0002:
                    chip8_and_vx_vy(c); 
                    break;
                case 0x0003:
                    chip8_xor_vx_vy(c); 
                    break;
                case 0x0004:
                    chip8_add_vx_vy(c); 
                    break;
                case 0x0005:
                    chip8_sub_vx_vy(c); 
                    break;
                case 0x0006:
                    chip8_shr_vx(c); 
                    break;
                case 0x0007:
                    chip8_subn_vx_vy(c); 
                    break;
                case 0x000E:
                    chip8_shl_vx_vy(c); 
                    break;
                default:
                    printf("ERROR: Unrecognized opcode 0x%X\n", c->opcode);
                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 0x9000:
            chip8_sne_vx_vy(c);
            break;
        case 0xA000:
            chip8_ld_i(c);
            break;
        case 0xB000:
            chip8_jmp_v0(c);
            break;
        case 0xC000:
            chip8_rnd_v0(c);
            break;
        case 0xD000:
            chip8_drw(c);
            break;
        case 0xE000:
            switch(c->opcode & 0x00FF) {
                case 0x009E:
                    chip8_skp(c);
                    break;
                case 0x00A1:
                    chip8_sknp(c);
                    break;
                default:
                    printf("ERROR: Unrecognized opcode 0x%X\n", c->opcode);
                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 0xF000:
            switch(c->opcode & 0x00FF) {
                case 0x0007:
                    chip8_ld_vx(c);
                    break;
                case 0x000A:
                    chip8_ld_vx_k(c);
                    break;
                case 0x0015:
                    chip8_ld_dt_vx(c);
                    break;
                case 0x0018:
                    chip8_ld_st_vx(c);
                    break;
                case 0x001E:
                    chip8_add_i_vx(c);
                    break;
                case 0x0029:
                    chip8_ld_f_vx(c);
                    break;
                case 0x0033:
                    chip8_ld_b_vx(c);
                    break;
                case 0x0055:
                    chip8_fx55(c);
                    break;
                case 0x0065:
                    chip8_fx65(c);
                    break;
                default:
                    printf("ERROR: Unrecognized opcode 0x%X\n", c->opcode);
                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        default:
            printf("ERROR: Unrecognized opcode 0x%X\n", c->opcode);
            break;
    }
}

uint8_t chip8_get_vx(Chip8* c)
{
    return (c->opcode & 0x0F00u) >> 8;
}

uint8_t chip8_get_vy(Chip8* c)
{
	return (c->opcode & 0x00F0u) >> 4;
}

void chip8_cls(Chip8* c)
{
    memset(c->screen, 0, sizeof(c->screen));
}

void chip8_ret(Chip8* c)
{
    c->pc = c->stack[--c->sp];
}

void chip8_jmp(Chip8* c)
{
    c->pc = c->opcode & 0x0FFF;    
}

void chip8_call(Chip8* c)
{
    uint16_t address = c->opcode & 0x0fff;
    c->stack[c->sp] = c->pc;
    c->sp += 1;
    c->pc = address;
}

void chip8_se_vx(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t byte = c->opcode & 0x00FF;

    if (c->registers[vx] == byte) {
        c->pc += 2;
    }
}

void chip8_sne_vx(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t byte = c->opcode & 0x00FF;

    if (c->registers[vx] != byte) {
        c->pc += 2;
    }
}

void chip8_se_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);

	if (c->registers[vx] == c->registers[vy]) {
		c->pc += 2;
	}   
}

void chip8_ld_vx(Chip8* c)
{
    uint8_t Vx = (c->opcode & 0x0F00u) >> 8;
	uint8_t byte = c->opcode & 0x00FFu;

	c->registers[Vx] = byte;
}

void chip8_add_vx(Chip8* c)
{
    uint8_t vx = c->opcode & 0x0F00 >> 8;
    uint8_t byte = c->opcode & 0x00FF;

    c->registers[vx] = byte;
}

void chip8_ld_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);
    c->registers[vx] = c->registers[vy];
}

void chip8_or_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);
    c->registers[vx] |= c->registers[vy];
}

void chip8_and_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);
    c->registers[vx] &= c->registers[vy];
}

void chip8_xor_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);
    c->registers[vx] ^= c->registers[vy];
}

void chip8_add_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);

    uint16_t sum = c->registers[vx] + c->registers[vy];

    if (sum > 255) {
        c->registers[0xF] = 1;
    } else {
        c->registers[0xF] = 0;
    }
    c->registers[vx] = sum & 0xFF;
}

void chip8_sub_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);

    if (c->registers[vx] > c->registers[vy]) {
        c->registers[0xF] = 1;
    } else {
        c->registers[0xF] = 0;
    }
    c->registers[vx] -= c->registers[vy];
}

void chip8_shr_vx(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);

    c->registers[0xF] = c->registers[vx] & 0x1;
    c->registers[vx] >>= 1;
}

void chip8_subn_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);

    if (c->registers[vy] > c->registers[vx]) {
        c->registers[0xF] = 1;
    } else {
        c->registers[0xF] = 0;
    }
    c->registers[vx] = c->registers[vy] - c->registers[vx];
}

void chip8_shl_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);

    c->registers[0xF] = c->registers[vx] & 0x80 >> 7;
    c->registers[vx] <<= 1;
}

void chip8_sne_vx_vy(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);

    if (c->registers[vx] != c->registers[vy]) {
        c->pc += 2;
    }
}

void chip8_ld_i(Chip8* c)
{
    uint16_t address = c->opcode & 0x0FFF;
    c->index = address;
}

void chip8_jmp_v0(Chip8* c)
{
    uint16_t address = c->opcode & 0x0FFF;
    c->pc = c->registers[0] + address;
}

void chip8_rnd_v0(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t byte = c->opcode & 0x00FF;

    c->registers[vx] = rand() % 256 & byte;
}

void chip8_drw(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t vy = chip8_get_vy(c);
    uint8_t height = c->opcode & 0x000F;
    uint8_t x_pos = c->registers[vx];
    uint8_t y_pos = c->registers[vy];

    c->registers[0xF] = 0;

    for (size_t row = 0; row < height; row++) {
        uint8_t sprite_byte = c->memory[c->index + row];
        for (size_t col = 0; col < 8; col++) {

            uint8_t sprite_pixel = sprite_byte & (0x80 >> col);
            uint32_t* screen_pixel = &c->screen[(y_pos + row) * VIDEO_WIDTH + (x_pos + col)];

            if (sprite_pixel) {
                if (*screen_pixel == 0xFFFFFFFF) {
                    c->registers[0xF] = 1;
                }
                *screen_pixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void chip8_skp(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t key = c->registers[vx];
    if (c->keypad[key]) {
        c->pc += 2;
    }
}
void chip8_sknp(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t key = c->registers[vx];
    if (!c->keypad[key]) {
        c->pc += 2;
    }
    
}

void chip8_ld_vx_dt(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    c->registers[vx] = c->delay_timer;
}

void chip8_ld_vx_k(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);

    if (c->keypad[0]) {
        c->registers[vx] = 0;
    } else if (c->keypad[1]) {
        c->registers[vx] = 1;
    } else if (c->keypad[2]) {
        c->registers[vx] = 2;
    } else if (c->keypad[3]) {
        c->registers[vx] = 3;
    } else if (c->keypad[4]) {
        c->registers[vx] = 4;
    } else if (c->keypad[5]) {
        c->registers[vx] = 5;
    } else if (c->keypad[6]) {
        c->registers[vx] = 6;
    } else if (c->keypad[7]) {
        c->registers[vx] = 7;
    } else if (c->keypad[8]) {
        c->registers[vx] = 8;
    } else if (c->keypad[9]) {
        c->registers[vx] = 9;
    } else if (c->keypad[10]) {
        c->registers[vx] = 10;
    } else if (c->keypad[11]) {
        c->registers[vx] = 11;
    } else if (c->keypad[12]) {
        c->registers[vx] = 12;
    } else if (c->keypad[13]) {
        c->registers[vx] = 13;
    } else if (c->keypad[14]) {
        c->registers[vx] = 14;
    } else if (c->keypad[15]) {
        c->registers[vx] = 15;
    } else {
        c->pc += 2;
    }
}

void chip8_ld_dt_vx(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    c->delay_timer = c->registers[vx];
}

void chip8_ld_st_vx(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    c->sound_timer = c->registers[vx];
}

void chip8_add_i_vx(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    c->index += c->registers[vx];
}

void chip8_ld_f_vx(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t digit = c->registers[vx];

    c->index = FONTSET_START_ADDRESS + (5 * digit);
}

void chip8_ld_b_vx(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);
    uint8_t value = c->registers[vx];

    c->memory[c->index + 2] = value % 10;
    value /= 10;
    c->memory[c->index + 1] = value % 10;
    value /= 10;
    c->memory[c->index] = value % 10;
}

void chip8_fx55(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);

    for (uint8_t i = 0; i < vx; ++i) {
        c->memory[c->index + 1] = c->registers[i];
    }
}
void chip8_fx65(Chip8* c)
{
    uint8_t vx = chip8_get_vx(c);

    for (uint8_t i = 0; i < vx; ++i) {
        c->registers[i] = c->memory[c->index + i];
    }
}
