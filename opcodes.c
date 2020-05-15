#include <stdlib.h>
#include <stdio.h>
#ifndef CPU_H
#define CPU_H
#include "cpu.h"
#endif

#define COMPARE_RESULT_GT 1
#define COMPARE_RESULT_LT 2
#define COMPARE_RESULT_EQ 3



uint32_t opcode_0_noop(instruction currentInstruction, systemcpu *cpu){
	//printf("bah");
	return 0;
};

uint32_t opcode_1_store_mem(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]){
	//printf("store_mem\n");
	uint32_t value = payload[0];
	uint32_t location = payload[1];
	//printf("Value: %d  Location: %d\n", value, location);
	cpu->mem->memory[location] = value;
	return 0;
}

uint32_t opcode_2_copy_mem(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]){
	// arg1 sets the mode of the copy
	// - 0: value at memory[source] is copied
	// - non-zero: value at memory[memory[source]] is copied
	uint32_t source = payload[0];
	uint32_t destination = payload[1];
	if (currentInstruction.arg1 == 0){
		cpu->mem->memory[destination] = cpu->mem->memory[source];
	} else {
		cpu->mem->memory[destination] = cpu->mem->memory[cpu->mem->memory[source]];
	}
	
	return 0;
}

uint32_t opcode_3_add(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]){
	// arg1 sets the mode of the add
	// - 0: both a and b are memory locations
	// - non-zero: a is a mem location, b is an explicit value
	uint32_t a = payload[0];
	uint32_t b = payload[1];
	uint32_t destination = payload[2];
	//printf("Add: arg1: %d a: %d b: %d destination: %d\n", currentInstruction.arg1, a, b, destination);
	if (currentInstruction.arg1 == 0){
		//printf("Treating both as locations. Value at a: %d Value at b: %d\n", cpu->mem->memory[a], cpu->mem->memory[b]);
		cpu->mem->memory[destination] = cpu->mem->memory[a] + cpu->mem->memory[b];
	} else {
		cpu->mem->memory[destination] = cpu->mem->memory[a] + b;
	}
	//printf("Add: value at destination: %d\n", cpu->mem->memory[destination]);
	return 0;
}

uint32_t opcode_4_jump(instruction currentInstruction, systemcpu *cpu, uint32_t payload[1]){
	uint32_t jump_destination = payload[0];
	uint32_t return_to = cpu->ip;
	stack_push(cpu, return_to, return_to, jump_destination);
	cpu->ip = jump_destination - 1;
	return 0;
}

uint32_t opcode_5_return(instruction currentInstruction, systemcpu *cpu){
	stack_pop(cpu);
	return 0;
}

uint32_t opcode_6_goto(instruction currentInstruction, systemcpu *cpu, uint32_t payload[1]){
	uint32_t destination = payload[0];
	cpu->ip = destination - 1;
	return 0;
}

uint32_t opcode_7_halt(instruction currentInstruction, systemcpu *cpu){
	return CPU_STATUS_HALT;
}

uint32_t opcode_8_compare(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]){
	//instruction args determine if treated as explicit value or mem ref
	// arg1/2 = 0: treat payload[0/1] as explicit value
	// arg1/2 = non-zero: treat payload[0/1] as mem ref
	printf("Compare: arg1: %d arg2: %d p1: %d p2: %d\n", currentInstruction.arg1, currentInstruction.arg2, payload[0], payload[1]);
	uint32_t a, b;
	if (currentInstruction.arg1 == 0){
		a = payload[0];
	} else {
		a = cpu->mem->memory[payload[0]];
	}
	if (currentInstruction.arg2 == 0){
		b = payload[1];
	} else {
		b = cpu->mem->memory[payload[1]];
	}
	printf("Comparing a: %d b: %d\n", a, b);
	if (a > b){
		cpu->result = COMPARE_RESULT_GT;
	}
		if (a < b){
		cpu->result = COMPARE_RESULT_LT;
	}
		if (a == b){
		cpu->result = COMPARE_RESULT_EQ;
	}
	return 0;
}

uint32_t opcode_9_branch(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]){
	// arg1 in instruction used to determine if we jump or goto
	// 0 = goto, non-0 = jump w/ return
	uint32_t compare_value = payload[0];
	uint32_t jump_destination = payload[1];
	//printf("Branch: compare_value: %d jump_destination: %d\n", compare_value, jump_destination);
	if (cpu->result == compare_value){
		if (currentInstruction.arg1 == 0){
			//printf("Branch taken, goto style\n");
			cpu->ip = jump_destination - 1;
		} else {
			//printf("Branch taken, jump style\n");
			stack_push(cpu, cpu->ip, cpu->ip, jump_destination);
			cpu->ip = jump_destination - 1;
		}
		return 0;
	}
	//printf("Branch not taken\n");
	return 0;
}

uint32_t opcode_10_mult(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]){
	// arg1 sets the mode of the mult
	// - 0: both a and b are memory locations
	// - non-zero: a is a mem location, b is an explicit value
	uint32_t a = payload[0];
	uint32_t b = payload[1];
	uint32_t destination = payload[2];
	//printf("Mult: arg1: %d a: %d b: %d destination: %d\n", currentInstruction.arg1, a, b, destination);
	if (currentInstruction.arg1 == 0){
		//printf("Treating both as locations. Value at a: %d Value at b: %d\n", cpu->mem->memory[a], cpu->mem->memory[b]);
		cpu->mem->memory[destination] = cpu->mem->memory[a] * cpu->mem->memory[b];
	} else {
		cpu->mem->memory[destination] = cpu->mem->memory[a] * b;
	}
	//printf("Mult: value at destination: %d\n", cpu->mem->memory[destination]);
	return 0;
}

