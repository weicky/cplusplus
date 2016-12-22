#include <stdio.h>
#include <stdint.h>

void hex_print(uint8_t *data, size_t len)
{
	size_t i;
	for(i = 0; i < len; i++)
		printf("%0.2X%c", data[i], (i % 16 == 15 ? '\n' : ' '));
	printf("\n");
}