#include <stdio.h>

#define IN 1
#define OUT 0

int main(void) {
	int c, s;

	s = 0;
	while((c = getchar()) != EOF) {
		s = (c == ' ' || c == '\n' || c == '\t') ? s + 1 : 0;

		if(s > 1)
			continue;

		putchar(s == 1 ? '\n' : c);
	}

	return 0;
}
