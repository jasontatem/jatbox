#include <stdlib.h>

#define SYSTEM_MEM_SIZE 16777216
#define STACK_SIZE 100

typedef struct StackFrame {
	int32_t null_frame;
	int32_t called_from;
	int32_t return_to;
} stackframe;

typedef struct SystemMemory {
	int32_t *memory;
	stackframe stack[STACK_SIZE];
} systemmemory;



int32_t memory_init(systemmemory *mem);
stackframe null_frame(void);