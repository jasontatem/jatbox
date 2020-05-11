#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif
#include "cpu.h"

typedef struct SystemArch {
	systemcpu *cpu;
	systemmemory *memory;
} systemarch;

void system_init(systemarch *sys);
void system_tick(systemarch *sys);
