#include <stdio.h>

#define swap(t, x, y) { t __swapvar__; __swapvar__ = x; x = y; y = __swapvar__; }

int main(void) {
	int x = 32, y = 21;
	swap(int, x, y);
	printf("x = %i, y = %i\n", x, y);
	return 0;
}
