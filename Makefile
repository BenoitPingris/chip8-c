CC = gcc
LD = gcc

NAME = chip8

CFLAGS = -Wall -pipe 
OFLAGS = -c -I/usr/include
LFLAGS = $(CFLAGS) -L/usr/lib/

SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LFLAGS = $(shell sdl2-config --libs)

SOURCES = $(wildcard ./src/*.c)
OBJECTS = $(SOURCES:.c=.o)

DEBUG = no
PROFILE = no
PEDANTIC = no
OPTIMIZATION = -O3

ifeq ($(DEBUG), yes)
	CFLAGS += -g
	OPTIMIZATION = -O0
endif

ifeq ($(PROFILE), yes)
	CFLAGS += -pg
endif

CFLAGS += $(OPTIMIZATION)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(SDL_LFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(NAME)

rebuild: clean all

.PHONY : clean
