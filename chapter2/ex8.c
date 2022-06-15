#include <stdio.h>

int rightrot(int x, int n) {
	int firstn = x & ((1 << (n + 1)) - 1);
	x >>= n;
	return x | (firstn << ((sizeof(x) * 8) - n));
}

int main(void) {
	printf("%i\n", rightrot(1, 1));
	return 0;
}
