#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#ifndef CPU_H
#define CPU_H
#include "cpu.h"
#endif
#include "opcodes.h"
#include "io.h"
#include "log/log.h"

uint32_t cpu_init(systemcpu *cpu){
	cpu->sp = 0;
	cpu->ip = INITIAL_IP;
	cpu->err = 0;
	cpu->status = 0;
	cpu->result = 0;
	cpu->tick = 0;
	return 0;
};

uint32_t stack_push(systemcpu *cpu, uint32_t called_from, uint32_t return_to, uint32_t jumped_to){
	log_trace("PUSH: Stack Pointer: %d Called From: %d Return To: %d Jumped To: %d", cpu->sp, called_from, return_to, jumped_to);
	if (cpu->sp >= STACK_SIZE){
		cpu->err = ERR_STACK_OVERFLOW;
		return -1;
	}
	cpu->sp++;
	stackframe *newFrame = malloc(sizeof(stackframe));
	newFrame->null_frame = 0;
	newFrame->called_from = called_from;
	newFrame->return_to = return_to - 1;
	newFrame->jumped_to = jumped_to;
	cpu->mem->stack[cpu->sp] = *newFrame;
	systemmemory *bah = cpu->mem;
	stackframe bah2 = cpu->mem->stack[cpu->sp];
	uint32_t bah3 = cpu->mem->stack[cpu->sp].null_frame;
	log_trace("Topmost stackframe: Null: %d CF: %d RT: %d JT: %d", cpu->mem->stack[cpu->sp].null_frame, cpu->mem->stack[cpu->sp].called_from, cpu->mem->stack[cpu->sp].return_to, cpu->mem->stack[cpu->sp].jumped_to);
	return cpu->sp;
};

uint32_t stack_pop(systemcpu *cpu){
	log_trace("POP: Stack pointer: %d", cpu->sp);
	if (cpu->sp == 0){
		cpu->err = ERR_POP_EMPTY_STACK;
		return -1;
	}
	stackframe poppedFrame = cpu->mem->stack[cpu->sp];
	log_trace("Popped stackframe: Null: %d CF: %d RT: %d JT: %d", poppedFrame.null_frame, poppedFrame.called_from, poppedFrame.return_to, poppedFrame.jumped_to);
	if (poppedFrame.null_frame == 1){
		cpu->err = ERR_POP_NULL_FRAME;
		return -1;
	}
	cpu->ip = poppedFrame.return_to;
	stackframe *nf = null_frame();
	cpu->mem->stack[cpu->sp] = *nf;
	cpu->sp--;
	return cpu->sp;
}

instruction decode_instruction(uint32_t raw_instruction){
	instruction newInstruction;
	newInstruction.opcode = extract_bits(raw_instruction, 8, 1);
	newInstruction.payload_len = extract_bits(raw_instruction, 8, 9);
	newInstruction.arg1 = extract_bits(raw_instruction, 8, 17);
	newInstruction.arg2 = extract_bits(raw_instruction, 8, 25);
    log_trace("Raw: %d  Opcode: %d  Len: %d  Arg1: %d  Arg2: %d", raw_instruction, newInstruction.opcode, newInstruction.payload_len, newInstruction.arg1, newInstruction.arg2);
	return newInstruction;
}

void opcode_dispatcher(instruction currentInstruction, systemcpu *cpu){
	uint32_t payload[currentInstruction.payload_len];
	if (currentInstruction.payload_len > 0){
		for (int i=0; i < currentInstruction.payload_len; i++){
			payload[i] = cpu->mem->memory[cpu->ip + 1];
			cpu->ip++;
		}
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
			break;
		case 4: // Jump
			cpu->status = opcode_4_jump(currentInstruction, cpu, payload);
			break;
		case 5: // Return
			cpu->status = opcode_5_return(currentInstruction, cpu);
			break;
		case 6: // Goto
			cpu->status = opcode_6_goto(currentInstruction, cpu, payload);
			break;
		case 7: // Halt
			cpu->status = opcode_7_halt(currentInstruction, cpu);
			break;
		case 8: // Compare
			cpu->status = opcode_8_compare(currentInstruction, cpu, payload);
			break;
		case 9: // Branch
			cpu->status = opcode_9_branch(currentInstruction, cpu, payload);
			break;
		case 10: // Multiply
			cpu->status = opcode_10_mult(currentInstruction, cpu, payload);
			break;
		case 11: // Divide
			cpu->status = opcode_11_div(currentInstruction, cpu, payload);
			break;
		case 12: // Subtract
			cpu->status = opcode_12_sub(currentInstruction, cpu, payload);
			break;
		case 13: // Pack8
			cpu->status = opcode_13_pack8(currentInstruction, cpu, payload);
			break;
		case 14: // Unpack8
			cpu->status = opcode_14_unpack8(currentInstruction, cpu, payload);
			break;
		case 15: // Pack16
			cpu->status = opcode_15_pack16(currentInstruction, cpu, payload);
			break;
		case 16: // Unpack16
			cpu->status = opcode_16_unpack16(currentInstruction, cpu, payload);
			break;
		case 17: // Clear
			cpu->status = opcode_17_clear(currentInstruction, cpu);
			break;
		case 18: // Memset
			cpu->status = opcode_18_memset(currentInstruction, cpu, payload);
			break;
		case 19: // Memcpy
			cpu->status = opcode_19_memcpy(currentInstruction, cpu, payload);
			break;	
		case 20: // Setpix
			cpu->status = opcode_20_setpix(currentInstruction, cpu, payload);
			break;	
		case 21: // Draw Bitmap
			cpu->status = opcode_21_drawbmap(currentInstruction, cpu, payload);
			break;
		case 22: // Draw Packed Bitmap
			cpu->status = opcode_22_drawbmap_p(currentInstruction, cpu, payload);
			break;	
		case 23: // Random Number
			cpu->status = opcode_23_rand(currentInstruction, cpu, payload);
			break;
		default:
			cpu->err = ERR_INVALID_OPCODE;
	}
	cpu->ip++;
};

void cpu_tick(systemcpu *cpu){
	log_trace("Tick started. SP: %d IP: %d", cpu->sp, cpu->ip);
	//printf("Valwatch: %d %d %d\n", cpu->mem->memory[2000000], cpu->mem->memory[2000007], cpu->mem->memory[2000006]);
	cpu->tick++;
	uint32_t next_instruction_raw = cpu->mem->memory[cpu->ip];
	instruction next_instruction = decode_instruction(next_instruction_raw);
	opcode_dispatcher(next_instruction, cpu);
	log_trace("Tick finished. SP: %d IP: %d", cpu->sp, cpu->ip); 
}