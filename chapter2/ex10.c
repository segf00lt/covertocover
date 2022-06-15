#include <stdio.h>

int lower(int c) {
	return c + ((c >= 'A' && c <= 'Z') ? 'a' - 'A' : 0);
}

int main(void) {
	printf("%c\n", lower('C'));
	return 0;
}
