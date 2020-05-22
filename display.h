#include <stdlib.h>
#include <SDL2/SDL.h>
#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif

typedef struct Display {
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_Texture *tex;
    uint32_t *pixels;
} display;

int display_init(display *d);
void display_stop(display *d);
void display_update(display *disp, systemmemory *mem);