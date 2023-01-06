#include <stdio.h>
#include "pico/stdlib.h"

uint16_t get_block(char *buffer, uint16_t buffer_len)
{
	uint16_t buffer_index = 0;
	while (buffer_index < buffer_len)
	{
		signed int c = getchar_timeout_us(100);
		if (c != PICO_ERROR_TIMEOUT)
		{
			buffer[buffer_index++] = (char)(c & 0xFF);
		}
	}
	return buffer_index;
}
