#include <stdlib.h>
#include <stdio.h>
#ifndef CPU_H
#define CPU_H
#include "cpu.h"
#endif
#include "opcodes.h"
#include "io.h"

int32_t cpu_init(systemcpu *cpu){
	cpu->sp = 0;
	cpu->ip = 0;
	cpu->err = 0;
	cpu->status = 0;
	cpu->result = 0;
	cpu->tick = 0;
	return 0;
};

int32_t stack_push(systemcpu *cpu, int32_t called_from, int32_t return_to){
	if (cpu->sp >= STACK_SIZE - 1){
		cpu->err = ERR_STACK_OVERFLOW;
		return -1;
	}
	stackframe newFrame;
	newFrame.null_frame = 0;
	newFrame.called_from = called_from;
	newFrame.return_to = return_to;
	cpu->mem->stack[cpu->sp] = newFrame;
	cpu->sp++;
	return cpu->sp;
};

int32_t stack_pop(systemcpu *cpu){
	if (cpu->sp == 0){
		cpu->err = ERR_POP_EMPTY_STACK;
		return -1;
	}
	stackframe poppedFrame;
	poppedFrame = cpu->mem->stack[cpu->sp - 1];
	if (poppedFrame.null_frame == 1){
		cpu->err = ERR_POP_NULL_FRAME;
		return -1;
	}
	cpu->ip = poppedFrame.return_to;
	stackframe nf = null_frame();
	cpu->mem->stack[cpu->sp - 1] = nf;
	cpu->sp--;
	return cpu->sp;
}

instruction decode_instruction(int32_t raw_instruction){
	instruction newInstruction;
	newInstruction.opcode = extract_bits(raw_instruction, 8, 1);
	newInstruction.payload_len = extract_bits(raw_instruction, 8, 9);
	newInstruction.arg1 = extract_bits(raw_instruction, 8, 17);
	newInstruction.arg2 = extract_bits(raw_instruction, 8, 25);
	//printf("Raw: %d  Opcode: %d  Len: %d  Arg1: %d  Arg2: %d\n", raw_instruction, newInstruction.opcode, newInstruction.payload_len, newInstruction.arg1, newInstruction.arg2);
	return newInstruction;
}

void opcode_dispatcher(instruction currentInstruction, systemcpu *cpu){
	int32_t payload[currentInstruction.payload_len];
	if (currentInstruction.payload_len > 0){
		for (int i=0; i < currentInstruction.payload_len; i++){
			payload[i] = cpu->mem->memory[cpu->ip + 1];
			cpu->ip++;
		}
	} else {
		payload[0] = 0;
	}
	switch(currentInstruction.opcode){
		case 0: // No-Op
			cpu->status = opcode_0_noop(currentInstruction, cpu);
			break;
		case 1: // Store Value to Memory
			cpu->status = opcode_1_store_mem(currentInstruction, cpu, payload);
			break;
		case 2: // Copy Value in Memory
			cpu->status = opcode_2_copy_mem(currentInstruction, cpu, payload);
			break;
		case 3: // Add
			cpu->status = opcode_3_add(currentInstruction, cpu, payload);
	}
	cpu->ip++;
};

void cpu_tick(systemcpu *cpu){
	cpu->tick++;
	int32_t next_instruction_raw = cpu->mem->memory[cpu->ip];
	instruction next_instruction = decode_instruction(next_instruction_raw);
	opcode_dispatcher(next_instruction, cpu);
	//printf("IP: %d\n", cpu->ip); 
}