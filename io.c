#include <stdlib.h>
#include <stdio.h>
#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif
#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif
#include "io.h"

int32_t io_dispatcher(systemmemory *mem){
	//printf("io_dispatcher\n");
	serial_console_handler(mem);
	return 0;
}

int32_t serial_console_handler(systemmemory *mem){
	// see what we have from the host
	//printf("serial handler start\n");
	int32_t send_field = mem->memory[SERIAL_CONSOLE_SEND_TO];
	int8_t data_sent = extract_bits(send_field, 8, 1);
	//printf("Send Field: %d Sent Flag: %d\n", send_field, data_sent);
	if (data_sent == 1){
		//printf("serial handler sees sent data\n");
		int8_t data_ack = extract_bits(send_field, 8, 9);
		if (data_ack == 0) {
			int8_t data_status = extract_bits(send_field, 8, 17);
			char data_value = extract_bits(send_field, 8, 25);
			printf("%c", data_value);
			for (int i=0; i<=6; i++){
				send_field = clearBit(send_field, 9 + i);
			}
			send_field = setBit(send_field, 16);
			mem->memory[SERIAL_CONSOLE_SEND_TO] = send_field;
		}

	}
	
}

