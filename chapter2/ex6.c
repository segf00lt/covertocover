#include <stdio.h>

int setbits(int x, int p, int n, int y) {
	return x | ((y & ((1 << (n + 1)) - 1) << p));
}

int main(void) {
	printf("%i\n", setbits(0, 1, 1, 3));
	return 0;
}
