#include <stdio.h>

int main(void) {
	printf("signed char: %i to %i\n",
			(signed char)(1 << (sizeof(signed char) * 7)),
			(signed char)(((1 << (sizeof(signed char) * 7)) - 1)));
	printf("unsigned char: 0 to %i\n", (1 << (sizeof(unsigned char) * 8)) - 1);
	printf("signed short: %i to %i\n",
			(signed short)(1 << (sizeof(signed short) * 8)),
			(signed short)(((1 << (sizeof(signed short) * 7)) - 1)));
	return 0;
}
