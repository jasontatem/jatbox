#include <stdlib.h>
#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif
#ifndef CPU_H
#define CPU_H
#include "cpu.h"
#endif


uint32_t opcode_0_noop(instruction currentInstruction, systemcpu *cpu);
uint32_t opcode_1_store_mem(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]);
uint32_t opcode_2_copy_mem(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]);
uint32_t opcode_3_add(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);
uint32_t opcode_4_jump(instruction currentInstruction, systemcpu *cpu, uint32_t payload[1]);
uint32_t opcode_5_return(instruction currentInstruction, systemcpu *cpu);
uint32_t opcode_6_goto(instruction currentInstruction, systemcpu *cpu, uint32_t payload[1]);
uint32_t opcode_7_halt(instruction currentInstruction, systemcpu *cpu);
uint32_t opcode_8_compare(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]);
uint32_t opcode_9_branch(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]);
uint32_t opcode_10_mult(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);



