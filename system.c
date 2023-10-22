#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <SDL2/SDL.h>
#include <sys/time.h>
#include "systemarch.h"
#include "log/log.h"
#include "util.h"
#ifndef DISPLAY_H
#define DISPLAY_H
#include "display.h"
#endif

#define DISP_REFRESH_MIN_TICKS 200

float timedifference_msec(struct timeval t0, struct timeval t1){
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

int load_bin(char* filename, systemarch *system, uint32_t start_loc){
	//printf("Mem pointers: %p %p\n", system->memory, system->memory->memory);
	//printf("1\n");
	FILE *myfile;
	myfile = fopen(filename, "rb");
	uint32_t i=start_loc;
	uint32_t temp;
	//printf("2\n");
	while(1){
		//printf("3\n");
		fread(&temp, sizeof(uint32_t), 1, myfile);
		//printf("4\n");
		if(feof(myfile)){
			//system->memory->memory[i] = 0;
			break;
		}
		//printf("Mem pointers: %p %p\n", system->memory, system->memory->memory);
		log_trace("%d Writing: %d", i, temp);
		system->memory->memory[i] = temp;
		//printf("Mem pointers: %p %p\n", system->memory, system->memory->memory);
		//printf("%d Reading: %d\n", i, system->memory->memory[i]);
		i++;
	}
	fclose(myfile);
	return i - start_loc;
}

char *cpu_stop_reason(cpu_status){
	if (cpu_status == CPU_STATUS_HALT){
		return "CPU Halted";
	} else if (cpu_status == CPU_STATUS_ERR){
		return "CPU Error";
	} else {
		return "Unknown status, check defines in cpu.h (or this is a bug)";
	}
}

char *cpu_err_reason(cpu_err){
	if (cpu_err == ERR_STACK_OVERFLOW){
		return "Stack overflow";
	} else if (cpu_err == ERR_INVALID_DATA){
		return "Invalid data";
	} else if (cpu_err == ERR_INVALID_OPCODE){
		return "Invalid opcode";
	} else if (cpu_err == ERR_POP_EMPTY_STACK){
		return "Attempted to pop empty stack";
	} else if (cpu_err == ERR_POP_NULL_FRAME){
		return "Null frame encountered during stack pop";
	}
	else {
		return "Unknown error, check defines in cpu.h (or this is a bug)";
	}
}

int main(void){
	srand(time(0));
	log_set_quiet(1);
	FILE *logfile;
	logfile = fopen("./jatbox.log", "w");
	log_set_fp(logfile);

	struct termios *orig_term_settings = malloc(sizeof(struct termios));
	getTerminalSettings(orig_term_settings);
	enableRawMode(orig_term_settings);

	printf("Starting...\n");
	systemarch *system0 = malloc(sizeof(systemarch));
	printf("Calling system_init\n");
	log_trace("System pointer before init: %p", system0);
	system_init(system0);
	printf("Calling memory_init\n");
	memory_init(system0->memory);
	printf("Calling cpu_init\n");
	cpu_init(system0->cpu);
	printf("Calling display_init\n");
	display_init(system0->disp);
	
	log_trace("System pointer after init: %p", system0);
	log_trace("Initial cpu pointer: %p", system0->cpu);
	log_trace("Initial mem pointer (system0.memory): %p", system0->memory);
	log_trace("Initial mem pointer (system0.memory->memory): %p", system0->memory->memory);
	log_trace("Initial mem pointer (system0.memory->stack): %p", system0->memory->stack);
	log_trace("Initial mem pointer (system0.cpu->mem->memory): %p", system0->cpu->mem->memory);
	log_trace("Initial mem pointer (system0.cpu->mem->stack): %p", system0->cpu->mem->stack);
	
	printf("Loading bios.bin...\n");
	int bios_size = load_bin("bios.bin", system0, 500000);
	printf("Loaded %d mem locations\n", bios_size);

	printf("Loading test.bin...\n");
	int prog_size = load_bin("test.bin", system0, 1000000);
	printf("Loaded %d mem locations\n", prog_size);

	printf("Starting CPU...\n");

	system0->cpu->ip = INITIAL_IP;

	struct timeval disp_last_refreshed;
	struct timeval current_time;

	gettimeofday(&disp_last_refreshed, 0);
	gettimeofday(&current_time, 0);

	struct timeval cpu_start_time;
	struct timeval cpu_stop_time;
	gettimeofday(&cpu_start_time, 0);

	struct timeval cpu_stat_last_update;
	gettimeofday(&cpu_stat_last_update, 0);

	uint32_t cpu_tick_last_val = 0;
	
	while(system0->cpu->status == 0){
		system0->tick++;
		if (system0->cpu->dsync != 1){
			system_tick(system0);
			if(system0->cpu->err != 0){
				printf("CPU error encountered: %d, %s\n", system0->cpu->err, cpu_err_reason(system0->cpu->err));
				system0->cpu->status = CPU_STATUS_ERR;
				break;
			}
		}
		
		if (system0->cpu->dsync == 1 || system0->cpu->tick % DISP_REFRESH_MIN_TICKS == 0){
			gettimeofday(&current_time, 0);
			float diff = timedifference_msec(disp_last_refreshed, current_time);
			if (diff >= 16.666666f){
				display_update(system0->disp, system0->memory);
				gettimeofday(&disp_last_refreshed, 0);
				//log_warn("Display Update: %f ms, CPU tick %d", diff, system0->cpu->tick);
				if (system0->cpu->dsync == 1){
					system0->cpu->dsync = 0;
				}
			}
		}

		if (system0->tick % 10000000 == 0) {
			gettimeofday(&current_time, 0);
			float diff = timedifference_msec(cpu_stat_last_update, current_time);
			float cpu_cycles_per_sec = (system0->cpu->tick - cpu_tick_last_val) / diff * 1000;
			float system_cycles_per_sec = 10000000 / diff * 1000;
			printf("%d system cycles: %f ms, %f system cycles/sec, %f cpu cycles/sec, cpu skipped %d cycles\n", system0->tick, diff, system_cycles_per_sec, cpu_cycles_per_sec, 10000000 - (system0->cpu->tick - cpu_tick_last_val));
			gettimeofday(&cpu_stat_last_update, 0);
			cpu_tick_last_val = system0->cpu->tick;
		}
		
		
	}
	gettimeofday(&cpu_stop_time, 0);
	printf("CPU reported non-zero status: %d, %s\n", system0->cpu->status, cpu_stop_reason(system0->cpu->status));
	
	float cpu_runtime = timedifference_msec(cpu_start_time, cpu_stop_time);
	printf("CPU was running for %f ms\n", cpu_runtime);

	fclose(logfile);
	disableRawMode(orig_term_settings);
	display_stop(system0->disp);
	return 0;
}