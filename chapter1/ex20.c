/* detab */

#include <stdio.h>

#define TABWIDTH 8

int main(void) {
	int c;
	int lastn, space;

	lastn = space = 0;

	while((c = getchar()) != EOF) {
		switch(c) {
		case '\t':
			space = (lastn == TABWIDTH) ? TABWIDTH : (TABWIDTH - lastn);
			for(; space > 0; --space)
				putchar(' ');
			lastn = 0;
			break;
		case '\n':
			lastn = 0;
			putchar(c);
			break;
		default:
			++lastn;
			putchar(c);
		}
	}

	return 0;
}
