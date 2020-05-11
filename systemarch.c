#include <stdlib.h>
#include <stdio.h>
#include "systemarch.h"
#include "io.h"

void system_init(systemarch *sys){
	printf("System pointer at init call: %p\n", sys);
	//systemarch *sys = malloc(sizeof(systemarch));
	systemcpu *cpu0 = malloc(sizeof(systemcpu));
	systemmemory *memory0 = malloc(sizeof(systemmemory));
	int32_t *memptr = malloc(sizeof(int32_t) * SYSTEM_MEM_SIZE);
	memory0->memory = memptr;
	stackframe *stackptr = malloc(sizeof(stackframe) * STACK_SIZE);
	memory0->stack = stackptr;
	cpu0->mem = memory0;
	sys->cpu = cpu0;
	sys->memory = memory0;
	/*printf("Pre Mem Init State:\n");
	printf("sys: %p\n", sys);
	printf("sys->cpu: %p\n", sys->cpu);
	printf("sys->cpu->mem: %p\n", sys->cpu->mem);
	printf("sys->cpu->mem->memory: %p\n", sys->cpu->mem->memory);
	printf("sys->cpu->mem->stack: %p\n", sys->cpu->mem->stack);
	printf("sys->memory: %p\n", sys->memory);
	printf("sys->memory->memory: %p\n", sys->memory->memory);
	printf("sys->memory->stack: %p\n", sys->memory->stack);
	//memory_init(sys->memory);
	printf("Post Mem Init State:\n");
	printf("sys: %p\n", sys);
	printf("sys->cpu: %p\n", sys->cpu);
	printf("sys->cpu->mem: %p\n", sys->cpu->mem);
	printf("sys->cpu->mem->memory: %p\n", sys->cpu->mem->memory);
	printf("sys->cpu->mem->stack: %p\n", sys->cpu->mem->stack);
	printf("sys->memory: %p\n", sys->memory);
	printf("sys->memory->memory: %p\n", sys->memory->memory);
	printf("sys->memory->stack: %p\n", sys->memory->stack);
	//cpu_init(sys->cpu);
	*/
};

void system_tick(systemarch *sys){
	cpu_tick(sys->cpu);
	io_dispatcher(sys->memory);
}
