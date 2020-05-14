#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "memory.h"


void memory_init(systemmemory *mem){
	//printf("State at beginning of memory_init:\n");
	//printf("mem: %p\n", mem);
	//printf("mem->memory: %p\n", mem->memory);
	//printf("mem->stack: %p\n", mem->stack);
	int i;
	for (i=0; i <= SYSTEM_MEM_SIZE; i++){
		mem->memory[i] = 0;
	}
	//printf("State after mem clear during memory_init:\n");
	//printf("mem: %p\n", mem);
	//printf("mem->memory: %p\n", mem->memory);
	//printf("mem->stack: %p\n", mem->stack);
	for (i=0; i <= STACK_SIZE; i++){
		stackframe *nf = null_frame();
		mem->stack[i] = *nf;
	}
	//printf("State at end of memory_init:\n");
	//printf("mem: %p\n", mem);
	//printf("mem->memory: %p\n", mem->memory);
	//printf("mem->stack: %p\n", mem->stack);
};

stackframe *null_frame(void){
	stackframe *nullFrame = malloc(sizeof(stackframe));
	nullFrame->null_frame = 1;
	return nullFrame;
}

