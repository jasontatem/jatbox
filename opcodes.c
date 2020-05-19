#include <stdlib.h>
#include <stdio.h>
#ifndef CPU_H
#define CPU_H
#include "cpu.h"
#endif
#include "log/log.h"

#define COMPARE_RESULT_GT 1
#define COMPARE_RESULT_LT 2
#define COMPARE_RESULT_EQ 3



uint32_t opcode_0_noop(instruction currentInstruction, systemcpu *cpu){
	//printf("bah");
	return 0;
};

uint32_t opcode_1_store_mem(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]){ 
	uint32_t value = payload[0];
	uint32_t location = payload[1];
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
	log_trace("Add: arg1: %d a: %d b: %d destination: %d", currentInstruction.arg1, a, b, destination);
	if (currentInstruction.arg1 == 0){
		log_trace("Treating both as locations. Value at a: %d Value at b: %d", cpu->mem->memory[a], cpu->mem->memory[b]);
		cpu->mem->memory[destination] = cpu->mem->memory[a] + cpu->mem->memory[b];
	} else {
		cpu->mem->memory[destination] = cpu->mem->memory[a] + b;
	}
	log_trace("Add: value at destination: %d", cpu->mem->memory[destination]);
	return 0;
}

uint32_t opcode_4_jump(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]){
	// If payload[1] is non-zero, we treat that as a custom return location rather than returning to the current IP
	uint32_t jump_destination = payload[0];
	uint32_t return_to;
	if (payload[1] == 0){
		return_to = cpu->ip;
	} else {
		return_to = payload[1];
	}
	stack_push(cpu, cpu->ip, return_to, jump_destination);
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
	log_trace("Compare: arg1: %d arg2: %d p1: %d p2: %d", currentInstruction.arg1, currentInstruction.arg2, payload[0], payload[1]);
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
	log_trace("Comparing a: %d b: %d", a, b);
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

uint32_t opcode_9_branch(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]){
	// arg1 in instruction used to determine if we jump or goto
	// 0 = goto, non-0 = jump w/ return
	// if payload[2] is non-zero, treat that as a custom return location
	uint32_t compare_value = payload[0];
	uint32_t jump_destination = payload[1];
	log_trace("Branch: compare_value: %d jump_destination: %d", compare_value, jump_destination);
	if (cpu->result == compare_value){
		if (currentInstruction.arg1 == 0){
			log_trace("Branch taken, goto style");
			cpu->ip = jump_destination - 1;
		} else {
			log_trace("Branch taken, jump style");
			int32_t return_to;
			if (payload[2] == 0){
				return_to = cpu->ip;
			} else {
				return_to = payload[2];
			}
			stack_push(cpu, cpu->ip, return_to, jump_destination);
			cpu->ip = jump_destination - 1;
		}
		return 0;
	}
	log_trace("Branch not taken");
	return 0;
}

uint32_t opcode_10_mult(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]){
	// arg1 sets the mode of the mult
	// - 0: both a and b are memory locations
	// - non-zero: a is a mem location, b is an explicit value
	uint32_t a = payload[0];
	uint32_t b = payload[1];
	uint32_t destination = payload[2];
	log_trace("Mult: arg1: %d a: %d b: %d destination: %d", currentInstruction.arg1, a, b, destination);
	if (currentInstruction.arg1 == 0){
		log_trace("Treating both as locations. Value at a: %d Value at b: %d", cpu->mem->memory[a], cpu->mem->memory[b]);
		cpu->mem->memory[destination] = cpu->mem->memory[a] * cpu->mem->memory[b];
	} else {
		cpu->mem->memory[destination] = cpu->mem->memory[a] * b;
	}
	log_trace("Mult: value at destination: %d", cpu->mem->memory[destination]);
	return 0;
}

uint32_t opcode_11_div(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]){
	// arg1 sets the mode of the divide
	// - 0: both a and b are memory locations
	// - non-zero: a is a mem location, b is an explicit value
	uint32_t a = payload[0];
	uint32_t b = payload[1];
	uint32_t destination = payload[2];
	log_trace("Div: arg1: %d a: %d b: %d destination: %d", currentInstruction.arg1, a, b, destination);
	if (currentInstruction.arg1 == 0){
		log_trace("Treating both as locations. Value at a: %d Value at b: %d", cpu->mem->memory[a], cpu->mem->memory[b]);
		cpu->mem->memory[destination] = cpu->mem->memory[a] / cpu->mem->memory[b];
	} else {
		cpu->mem->memory[destination] = cpu->mem->memory[a] / b;
	}
	log_trace("Div: value at destination: %d", cpu->mem->memory[destination]);
	return 0;
}

uint32_t opcode_12_sub(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]){
	// arg1 sets the mode of the add
	// - 0: both a and b are memory locations
	// - non-zero: a is a mem location, b is an explicit value
	uint32_t a = payload[0];
	uint32_t b = payload[1];
	uint32_t destination = payload[2];
	log_trace("Sub: arg1: %d a: %d b: %d destination: %d", currentInstruction.arg1, a, b, destination);
	if (currentInstruction.arg1 == 0){
		log_trace("Treating both as locations. Value at a: %d Value at b: %d", cpu->mem->memory[a], cpu->mem->memory[b]);
		cpu->mem->memory[destination] = cpu->mem->memory[a] - cpu->mem->memory[b];
	} else {
		cpu->mem->memory[destination] = cpu->mem->memory[a] - b;
	}
	log_trace("Sub: value at destination: %d", cpu->mem->memory[destination]);
	return 0;
}

uint32_t opcode_13_pack8(instruction currentInstruction, systemcpu *cpu, uint32_t payload[5]){
	// arg1 determines whether the source vals are mem locations or explicit values
	// - 0: mem location
	// - non-zero: explicit value
	uint32_t result = 0;
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	if (currentInstruction.arg1 == 0){
		a = cpu->mem->memory[payload[0]];
		b = cpu->mem->memory[payload[1]];
		c = cpu->mem->memory[payload[2]];
		d = cpu->mem->memory[payload[3]];
	} else {
		a = payload[0];
		b = payload[1];
		c = payload[2];
		d = payload[3];
	}
	uint32_t destination = payload[4];
	if (a > 255 || b > 255 || c > 255 || d > 255){
		cpu->err = ERR_INVALID_DATA;
		return CPU_STATUS_ERR;
	}
	result += a;
	result += (b << 8);
	result += (c << 16);
	result += (d << 24);
	log_trace("Packing 8bit vals %d %d %d %d, result %d", a, b, c, d, result);
	cpu->mem->memory[destination] = result;
	return 0;
}

uint32_t opcode_14_unpack8(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]){
	// arg1 determines whether the source is a mem location or explicit vlaue
	// - 0: mem location
	// - non-zero: explicit value
	uint32_t source;
	if (currentInstruction.arg1 == 0){
		source = cpu->mem->memory[payload[0]];
	} else {
		source = payload[0];
	}
	uint32_t destination = payload[1];
	uint32_t a = extract_bits(source, 8, 1);
	uint32_t b = extract_bits(source, 8, 9);
	uint32_t c = extract_bits(source, 8, 17);
	uint32_t d = extract_bits(source, 8, 25);
	log_trace("Unpacked 8bit vals %d %d %d %d from %d", a, b, c, d, source);
	cpu->mem->memory[destination] = a;
	cpu->mem->memory[destination + 1] = b;
	cpu->mem->memory[destination + 2] = c;
	cpu->mem->memory[destination + 3] = d;
	return 0;
}

uint32_t opcode_15_pack16(instruction currentInstruction, systemcpu *cpu, uint32_t payload[3]){
	// arg1 determines whether the source vals are mem locations or explicit values
	// - 0: mem location
	// - non-zero: explicit value
	uint32_t result = 0;
	uint32_t a;
	uint32_t b;
	if (currentInstruction.arg1 == 0){
		a = cpu->mem->memory[payload[0]];
		b = cpu->mem->memory[payload[1]];
	} else {
		a = payload[0];
		b = payload[1];
	}
	uint32_t destination = payload[2];
	if (a > 65535 || b > 65535 ){
		cpu->err = ERR_INVALID_DATA;
		return CPU_STATUS_ERR;
	}
	result += a;
	result += (b << 16);
	log_trace("Packing 16bit vals %d %d, result %d", a, b, result);
	cpu->mem->memory[destination] = result;
	return 0;
}

uint32_t opcode_16_unpack16(instruction currentInstruction, systemcpu *cpu, uint32_t payload[2]){
	// arg1 determines whether the source is a mem location or explicit vlaue
	// - 0: mem location
	// - non-zero: explicit value
	uint32_t source;
	if (currentInstruction.arg1 == 0){
		source = cpu->mem->memory[payload[0]];
	} else {
		source = payload[0];
	}
	uint32_t destination = payload[1];
	uint32_t a = extract_bits(source, 16, 1);
	uint32_t b = extract_bits(source, 16, 17);
	log_trace("Unpacked 16bit vals %d %d from %d", a, b, source);
	cpu->mem->memory[destination] = a;
	cpu->mem->memory[destination + 1] = b;
	return 0;
}

uint32_t opcode_17_clear(instruction currentInstruction, systemcpu *cpu){
	// Clears Status and Result registers on cpu. Status cleared by return
	cpu->result = 0;
	return 0;
}

