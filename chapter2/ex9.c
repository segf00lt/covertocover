#include <stdio.h>

int bitcount(unsigned int x) {
	int b = 0;
	for(; x; ++b)
		x &= (x-1);
	return b;
}

int main(void) {
	printf("%i\n", bitcount(7));
	return 0;
}
