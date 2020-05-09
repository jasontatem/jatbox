#include <stdlib.h>
#include <stdio.h>
#include "systemarch.h"

void load_bin(char* filename, systemarch *system){
	FILE *myfile;
	myfile = fopen(filename, "rb");
	int i=0;
	int32_t temp;
	while(1){
		fread(&temp, sizeof(int32_t), 1, myfile);
		printf("%d Writing: %d\n", i, temp);
		system->memory->memory[i] = temp;
		printf("%d Reading: %d\n", i, system->memory->memory[i]);
		if(feof(myfile)){
			system->memory->memory[i] = 0;
			break;
		}
		i++;
	}
	fclose(myfile);
}

int main(void){
	systemarch system0;
	system0 = system_init();

	load_bin("test.bin", &system0);
	
	while(1){
		system_tick(system0);
		/*printf("Tick: %d\n", system0.cpu->tick);*/
	}
	return 0;
}