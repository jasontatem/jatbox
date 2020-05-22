#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct Display {
    SDL_Window *win;
    SDL_Renderer *ren;
} display;

int display_init(display *d);
void display_stop(display *d);