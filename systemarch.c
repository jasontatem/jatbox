#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "systemarch.h"
#include "io.h"

void system_init(systemarch *sys){
	//printf("System pointer at init call: %p\n", sys);
	systemcpu *cpu0 = malloc(sizeof(systemcpu));
	systemmemory *memory0 = malloc(sizeof(systemmemory));
	uint32_t *memptr = malloc(sizeof(uint32_t) * SYSTEM_MEM_SIZE);
	memory0->memory = memptr;
	stackframe *stackptr = malloc(sizeof(stackframe) * STACK_SIZE);
	memory0->stack = stackptr;
	cpu0->mem = memory0;
	sys->cpu = cpu0;
	sys->memory = memory0;
};

void system_tick(systemarch *sys){
	cpu_tick(sys->cpu);
	io_dispatcher(sys->memory);
}
