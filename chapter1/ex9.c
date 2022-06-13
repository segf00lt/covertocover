#include <stdio.h>

int main(void) {
	int c, s;

	s = 0;
	while((c = getchar()) != EOF) {
		s = (c == ' ') ? (s + 1) : 0;

		if(s <= 1)
			putchar(c);
	}

	return 0;
}
