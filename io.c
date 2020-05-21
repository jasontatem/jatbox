#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif
#ifndef UTIL_H
#define UTIL_H
#include "util.h"
#endif
#include "io.h"
#include "log/log.h"

uint32_t io_dispatcher(systemmemory *mem){
	//printf("io_dispatcher\n");
	serial_console_handler(mem);
	return 0;
}

uint32_t serial_console_handler(systemmemory *mem){
	// packing within uint32_t:
	// -- bits 1-8: data_sent flag
	// -- bits 9-16: data_ack flag
	// -- bits 17-24: data_status
	// -- bits 25-32: data_value
	// see what we have from the host
	//printf("serial handler start\n");
	uint32_t send_field = mem->memory[SERIAL_CONSOLE_SEND_TO];
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
	// see what we have from the terminal
	/*
	uint32_t recv_field = 0;
	char c = '\0';
	read(STDIN_FILENO, &c, 1);
	if (c == '\0'){
		return 0;
	} else {
		recv_field = c;
		recv_field += (1 << 24);
		log_trace("Char read from term: %c Value: %d Setting recv_field to %d", c, c, recv_field);
		mem->memory[SERIAL_CONSOLE_RECV_FROM] = recv_field;
	}
	*/
}

