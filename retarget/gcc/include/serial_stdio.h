#ifndef _SERIAL_STDIO_H
#define _SERIAL_STDIO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Serial_struct{
	char (* getChar)(void);
	void (* sendChar)(char);
} Serial_t;

void serial_puts(Serial_t  serial, const char * pString);
int serial_gets(Serial_t  serial, char * pBuffer, int bufferSize);

#define serial_printf(serial,...) {\
	char * __serial_temp_buffer;\
	asprintf(&__serial_temp_buffer, __VA_ARGS__);\
	serial_puts(serial,__serial_temp_buffer);\
	free(__serial_temp_buffer);\
}


#endif