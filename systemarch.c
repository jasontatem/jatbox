#include <stdlib.h>
#include "systemarch.h"
#include "io.h"

systemarch system_init(void){
	systemarch sys;
	systemcpu cpu0;
	systemmemory memory0;
	int32_t *ptr = malloc(sizeof(int32_t) * SYSTEM_MEM_SIZE);
	memory0.memory = ptr;
    cpu0.mem = &memory0;
	sys.cpu = &cpu0;
	sys.memory = &memory0;
	memory_init(sys.memory);
	cpu_init(sys.cpu);
	return sys;
};

void system_tick(systemarch sys){
	cpu_tick(sys.cpu);
	io_dispatcher(sys.memory);
}
