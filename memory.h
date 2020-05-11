#include <stdlib.h>

#define SYSTEM_MEM_SIZE 16777216
#define STACK_SIZE 200
#define IO_RANGE_LOW 0
#define IO_RANGE_HI 499999
#define BIOS_RANGE_LOW 500000
#define BIOS_RANGE_HI 999999
#define INITIAL_IP 1000000


typedef struct StackFrame {
	int32_t null_frame;
	int32_t called_from;
	int32_t return_to;
	int32_t jumped_to;
} stackframe;

typedef struct SystemMemory {
	stackframe *stack;
	int32_t *memory;
} systemmemory;



void memory_init(systemmemory *mem);
stackframe *null_frame(void);