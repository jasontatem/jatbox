#include <stdlib.h>
#include <stdio.h>
#include "systemarch.h"

void load_bin(char* filename, systemarch *system){
	printf("System pointer: %p\n", system);
	printf("Mem pointers: %p %p\n", system->memory, system->memory->memory);
	FILE *myfile;
	myfile = fopen(filename, "rb");
	int32_t i=INITIAL_IP;
	int32_t temp;
	while(1){
		fread(&temp, sizeof(int32_t), 1, myfile);
		if(feof(myfile)){
			//system->memory->memory[i] = 0;
			break;
		}
		//printf("Mem pointers: %p %p\n", system->memory, system->memory->memory);
		printf("%d Writing: %d\n", i, temp);
		system->memory->memory[i] = temp;
		//printf("Mem pointers: %p %p\n", system->memory, system->memory->memory);
		//printf("%d Reading: %d\n", i, system->memory->memory[i]);
		i++;
	}
	fclose(myfile);
}

int main(void){
	printf("Starting...\n");
	systemarch *system0 = malloc(sizeof(systemarch));
	printf("Calling system_init\n");
	printf("System pointer before init: %p\n", system0);
	system_init(system0);
	memory_init(system0->memory);
	printf("Calling cpu_init for cpu %p on system %p\n", system0->cpu, system0);
	cpu_init(system0->cpu);
	printf("System pointer after init: %p\n", system0);
	printf("Initial cpu pointer: %p\n", system0->cpu);
	printf("Initial mem pointer (system0.memory): %p\n", system0->memory);
	printf("Initial mem pointer (system0.memory->memory): %p\n", system0->memory->memory);
	printf("Initial mem pointer (system0.memory->stack): %p\n", system0->memory->stack);
	printf("Initial mem pointer (system0.cpu->mem->memory): %p\n", system0->cpu->mem->memory);
	printf("Initial mem pointer (system0.cpu->mem->stack): %p\n", system0->cpu->mem->stack);
	load_bin("test.bin", system0);


	system0->cpu->ip = INITIAL_IP;
	while(system0->cpu->status == 0){
		system_tick(system0);
			if(system0->cpu->err != 0){
				printf("CPU error encountered: %d\n", system0->cpu->err);
				system0->cpu->status = CPU_STATUS_ERR;
				break;
			}
		//printf("Tick: %d\n", system0.cpu->tick);
	}
	printf("CPU reported non-zero status: %d\n", system0->cpu->status);
	return 0;
}