#include <stdlib.h>
#include <stdint.h>
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
uint32_t opcode_4_jump(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]);
uint32_t opcode_5_return(instruction currentInstruction, systemcpu *cpu);
uint32_t opcode_6_goto(instruction currentInstruction, systemcpu *cpu, uint32_t payload[1]);
uint32_t opcode_7_halt(instruction currentInstruction, systemcpu *cpu);
uint32_t opcode_8_compare(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]);
uint32_t opcode_9_branch(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);
uint32_t opcode_10_mult(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);
uint32_t opcode_11_div(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);
uint32_t opcode_12_sub(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);
uint32_t opcode_13_pack8(instruction currentInstruction, systemcpu *cpu, uint32_t payload[5]);
uint32_t opcode_14_unpack8(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]);
uint32_t opcode_15_pack16(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);
uint32_t opcode_16_unpack16(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]);
uint32_t opcode_17_clear(instruction currentInstruction, systemcpu *cpu);
uint32_t opcode_18_memset(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);
uint32_t opcode_19_memcpy(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]);
uint32_t opcode_20_setpix(instruction currentInstruction, systemcpu *cpu, uint32_t payload[4]);
uint32_t opcode_21_drawbmap(instruction currentInstruction, systemcpu *cpu, uint32_t payload[6]);



