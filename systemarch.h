#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif
#include "cpu.h"

typedef struct SystemArch {
	systemcpu *cpu;
	systemmemory *memory;
} systemarch;

systemarch system_init(void);
void system_tick(systemarch sys);
