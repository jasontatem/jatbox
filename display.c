#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "display.h"

int display_init(display *d){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    d->win = SDL_CreateWindow("JatBox", 100, 100, 320, 240, SDL_WINDOW_SHOWN);
	if (d->win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	d->ren = SDL_CreateRenderer(d->win, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (d->ren == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		if (d->win != NULL) {
			SDL_DestroyWindow(d->win);
		}
		SDL_Quit();
		return EXIT_FAILURE;
	}
    return 0;
}

void display_stop(display *d){
    SDL_DestroyRenderer(d->ren);
	SDL_DestroyWindow(d->win);
	SDL_Quit();
}
