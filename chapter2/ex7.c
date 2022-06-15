#include <stdio.h>

int invert(int x, int p, int n) {
	return (n == 0) ? x : x ^ (((1 << n) - 1) << p);
}

int main(void) {
	printf("%i\n", invert(5, 1, 1));
	return 0;
}
