#include <stdlib.h>
#include <stdint.h>

#define SYSTEM_MEM_SIZE 16777216
#define STACK_SIZE 200
#define IO_RANGE_LOW 0
#define IO_RANGE_HI 499999
#define BIOS_RANGE_LOW 500000
#define BIOS_RANGE_HI 999999
#define INITIAL_IP 500000
#define VRAM_START 100000


typedef struct StackFrame {
	uint32_t null_frame;
	uint32_t called_from;
	uint32_t return_to;
	uint32_t jumped_to;
} stackframe;

typedef struct SystemMemory {
	stackframe *stack;
	uint32_t *memory;
} systemmemory;



void memory_init(systemmemory *mem);
stackframe *null_frame(void);