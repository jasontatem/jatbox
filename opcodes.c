#include <stdlib.h>
#include <stdio.h>
#ifndef CPU_H
#define CPU_H
#include "cpu.h"
#endif

#define COMPARE_RESULT_GT 1
#define COMPARE_RESULT_LT 2
#define COMPARE_RESULT_EQ 3



int32_t opcode_0_noop(instruction currentInstruction, systemcpu *cpu){
	//printf("bah");
	return 0;
};

int32_t opcode_1_store_mem(instruction currentInstruction, systemcpu *cpu, int32_t payload[2]){
	//printf("store_mem\n");
	int32_t value = payload[0];
	int32_t location = payload[1];
	//printf("Value: %d  Location: %d\n", value, location);
	cpu->mem->memory[location] = value;
	return 0;
}

int32_t opcode_2_copy_mem(instruction currentInstruction, systemcpu *cpu, int32_t payload[2]){
	int32_t source = payload[0];
	int32_t destination = payload[1];
	cpu->mem->memory[destination] = cpu->mem->memory[source];
	return 0;
}

int32_t opcode_3_add(instruction currentInstruction, systemcpu *cpu, int32_t payload[3]){
	int32_t a = payload[0];
	int32_t b = payload[1];
	int32_t destination = payload[2];
	cpu->mem->memory[destination] = a + b;
	return 0;
}

int32_t opcode_4_jump(instruction currentInstruction, systemcpu *cpu, int32_t payload[1]){
	int32_t jump_destination = payload[0];
	int32_t calling_location = cpu->ip;
	stack_push(cpu, calling_location, calling_location, jump_destination);
	cpu->ip = jump_destination - 1;
	return 0;
}

int32_t opcode_5_return(instruction currentInstruction, systemcpu *cpu){
	stack_pop(cpu);
	return 0;
}

int32_t opcode_6_goto(instruction currentInstruction, systemcpu *cpu, int32_t payload[1]){
	int32_t destination = payload[0];
	cpu->ip = destination - 1;
	return 0;
}

int32_t opcode_7_halt(instruction currentInstruction, systemcpu *cpu){
	return CPU_STATUS_HALT;
}

int32_t opcode_8_compare(instruction currentInstruction, systemcpu *cpu, int32_t payload[2]){
	if (payload[0] > payload[1]){
		return COMPARE_RESULT_GT;
	}
		if (payload[0] < payload[1]){
		return COMPARE_RESULT_LT;
	}
		if (payload[0] == payload[1]){
		return COMPARE_RESULT_EQ;
	}
	return 0;
}

int32_t opcode_9_branch(instruction currentInstruction, systemcpu *cpu, int32_t payload[2]){
	// arg1 in instruction used to determine if we jump or goto
	// 0 = goto, non-0 = jump w/ return
	// put 0 in status if we take the branch, 1 if we don't
	int32_t compare_value = payload[0];
	int32_t jump_destination = payload[1];
	if (cpu->status == compare_value){
		if (currentInstruction.arg1 == 0){
			cpu->ip = jump_destination - 1;
		} else {
			stack_push(cpu, cpu->ip, cpu->ip, jump_destination);
			cpu->ip = jump_destination - 1;
		}
		return 0;
	}
	return 1;
}

