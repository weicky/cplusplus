#include <stdio.h>
#include <stdint.h>

uint8_t hex2dec_char(char c)
{
	if(c > 47 && c < 59) //0~9
		return c - 48;
	else if(c > 64 && c < 71) //A~F
		return c - 55;
	else if(c > 96 && c < 103) //a~f
		return c - 87;
	else
		return 0;
}

uint8_t *hex2raw(const char *hex, uint8_t *buf)
{
	uint8_t c1, c2, *dup;
	int len, i;

	len = strlen(hex);
	if(!len || (len % 2) || !buf)
		return NULL;

	dup = buf;
    for(i = 0; i < len; i++)
	{
		c1 = hex2dec_char(*hex++);
		c2 = hex2dec_char(*hex++);
        *dup++ = (uint8_t)((c1 << 4) | c2);
    }

	return buf;
}

void hex_print(uint8_t *data, size_t len)
{
	size_t i;
	for(i = 0; i < len; i++)
		printf("%0.2X%c", data[i], (i % 16 == 15 ? '\n' : ' '));
	printf("\n");
}