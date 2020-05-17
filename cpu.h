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
	uint32_t result;  // result destination for instructions and subroutines
	uint32_t sp;  // stack pointer
	uint32_t ip;  // instruction pointer
	uint32_t err;  // error register
	uint32_t status;  // status register
	uint32_t tick;  // tick counter
	uint32_t halt;  // halt flag
	
	// system memory
	systemmemory *mem;
} systemcpu;

typedef struct Instruction {
	int8_t opcode;
	int8_t payload_len;
	int8_t arg1;
	int8_t arg2;
} instruction;

uint32_t cpu_init(systemcpu *cpu);
uint32_t stack_push(systemcpu *cpu, uint32_t called_from, uint32_t return_to, uint32_t jumped_to);
uint32_t stack_pop(systemcpu *cpu);
instruction decode_instruction(uint32_t raw_instruction);
void opcode_dispatcher(instruction currentInstruction, systemcpu *cpu);
void cpu_tick(systemcpu *cpu);

// Error Codes
#define ERR_STACK_OVERFLOW 1
#define ERR_POP_EMPTY_STACK 2
#define ERR_POP_NULL_FRAME 3
#define ERR_INVALID_OPCODE 4
#define ERR_INVALID_DATA 5

// Status Codes
#define CPU_STATUS_HALT 1
#define CPU_STATUS_ERR 2
