#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "display.h"
#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif
#include "log/log.h"
#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif

int display_init(display *d){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    d->win = SDL_CreateWindow("JatBox", 100, 100, 1280, 960, SDL_WINDOW_SHOWN);
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
	SDL_RenderSetScale(d->ren, 2.0, 2.0);
	SDL_RenderClear(d->ren);
	}
	//d->pixels = malloc(sizeof(uint32_t) * 320 * 240);
	//memset(d->pixels, 0, 320 * 240 * sizeof(uint32_t));
	d->tex = SDL_CreateTexture(d->ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 320, 240);
	log_trace("Display pointers at init: win: %p ren: %p tex: %p pix: %p", d->win, d->ren, d->tex, d->pixels);
    return 0;
}

void display_stop(display *disp){
	SDL_DestroyTexture(disp->tex);
    SDL_DestroyRenderer(disp->ren);
	SDL_DestroyWindow(disp->win);
	SDL_Quit();
}

void display_update(display *disp, systemmemory *mem){
	log_trace("Display pointers at update: win: %p ren: %p tex: %p pix: %p", disp->win, disp->ren, disp->tex, disp->pixels);
	//memcpy(disp->pixels, mem->memory[VRAM_START], sizeof(uint32_t) * 320 * 240);
	SDL_UpdateTexture(disp->tex, NULL, disp->pixels, 320 * sizeof(uint32_t));
	SDL_RenderClear(disp->ren);
	SDL_RenderCopy(disp->ren, disp->tex, NULL, NULL);
	SDL_RenderPresent(disp->ren);
}
