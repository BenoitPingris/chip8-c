#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MEMORY_SIZE 4096
#define STACK_SIZE 16
#define KEYPAD_SIZE 16
#define REGISTERS_SIZE 16
#define SCREEN_RESOLUTION 64*32
#define PROGRAM_START_ADDR 0x200
#define PROGRAM_END_ADDR 0xFFF
#define FONTSET_SIZE 80
#define FONTSET_START_ADDRESS 0x50
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

const static uint8_t fontset[FONTSET_SIZE] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

typedef struct Chip8 {
    uint8_t registers[REGISTERS_SIZE];
    uint8_t memory[MEMORY_SIZE];
    uint16_t index, pc, opcode;
    uint16_t stack[STACK_SIZE];
    uint8_t sp, delay_timer, sound_timer;
    uint8_t keypad[KEYPAD_SIZE];
    uint32_t screen[SCREEN_RESOLUTION];
	bool running;
} Chip8;

Chip8* chip8_new();
void chip8_load_rom(Chip8*, char *);
void chip8_free(Chip8*);
uint16_t chip8_fetch_opcode(Chip8*);
void chip8_cycle(Chip8*);
void chip8_execute_instructions(Chip8*);

uint8_t chip8_get_vx(Chip8*);
uint8_t chip8_get_vy(Chip8*);

void chip8_cls(Chip8*);
void chip8_ret(Chip8*);
void chip8_jmp(Chip8*);
void chip8_call(Chip8*);
void chip8_se_vx(Chip8*);
void chip8_sne_vx(Chip8*);
void chip8_se_vx_vy(Chip8*);
void chip8_ld_vx(Chip8*);
void chip8_add_vx(Chip8*);
void chip8_ld_vx_vy(Chip8*);
void chip8_or_vx_vy(Chip8*);
void chip8_and_vx_vy(Chip8*);
void chip8_xor_vx_vy(Chip8*);
void chip8_add_vx_vy(Chip8*);
void chip8_sub_vx_vy(Chip8*);
void chip8_shr_vx(Chip8*);
void chip8_subn_vx_vy(Chip8*);
void chip8_shl_vx_vy(Chip8*);
void chip8_sne_vx_vy(Chip8*);
void chip8_ld_i(Chip8*);
void chip8_jmp_v0(Chip8*);
void chip8_rnd_v0(Chip8*);
void chip8_drw(Chip8*);
void chip8_skp(Chip8*);
void chip8_sknp(Chip8*);
void chip8_ld_vx_dt(Chip8*);
void chip8_ld_vx_k(Chip8*);
void chip8_ld_dt_vx(Chip8*);
void chip8_ld_st_vx(Chip8*);
void chip8_add_i_vx(Chip8*);
void chip8_ld_f_vx(Chip8*);
void chip8_ld_b_vx(Chip8*);
void chip8_fx55(Chip8*);
void chip8_fx65(Chip8*);
