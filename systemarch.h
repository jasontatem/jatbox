#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif
#include "cpu.h"
#ifndef DISPLAY_H
#define DISPLAY_H
#include "display.h"
#endif

typedef struct SystemArch {
	systemcpu *cpu;
	systemmemory *memory;
	display *disp;
	uint32_t tick;
} systemarch;

void system_init(systemarch *sys);
void system_tick(systemarch *sys);
