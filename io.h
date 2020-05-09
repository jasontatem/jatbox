#include <stdlib.h>

#define SERIAL_CONSOLE_SEND_TO 16000000
#define SERIAL_CONSOLE_RECV_FROM 16000001

int32_t io_dispatcher(systemmemory *mem);
int32_t serial_console_handler(systemmemory *mem);