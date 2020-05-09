#include <stdlib.h>
#include <stdint.h>
#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif
#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif

typedef struct Cpu {
	// special purpose registers
	int32_t result;  // result destination for instructions and subroutines
	int32_t sp;  // stack pointer
	int32_t ip;  // instruction pointer
	int32_t err;  // error register
	int32_t status;  // status register
	int32_t tick;  // tick counter
	int32_t halt;  // halt flag
	
	// system memory
	systemmemory *mem;
} systemcpu;

typedef struct Instruction {
	int8_t opcode;
	int8_t payload_len;
	int8_t arg1;
	int8_t arg2;
} instruction;

int32_t cpu_init(systemcpu *cpu);
int32_t stack_push(systemcpu *cpu, int32_t called_from, int32_t return_to);
int32_t stack_pop(systemcpu *cpu);
instruction decode_instruction(int32_t raw_instruction);
void opcode_dispatcher(instruction currentInstruction, systemcpu *cpu);
void cpu_tick(systemcpu *cpu);

// Error Codes
#define ERR_STACK_OVERFLOW 1
#define ERR_POP_EMPTY_STACK 2
#define ERR_POP_NULL_FRAME 3
