#include <stdlib.h>
#include "memory.h"


int32_t memory_init(systemmemory *mem){
	int i;
	for (i=0; i <= SYSTEM_MEM_SIZE; i++){
		mem->memory[i] = 0;
	}
	stackframe nf = null_frame();
	mem->stack[0] = nf;
	return i;
};

stackframe null_frame(void){
	stackframe nullFrame;
	nullFrame.null_frame = 1;
	return nullFrame;
}

