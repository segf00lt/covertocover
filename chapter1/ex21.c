/* entab */

// TODO

#include <stdio.h>

#define TABWIDTH 8

int main(void) {
	int c;
	int lastn, tab, space;

	lastn = tab = space = 0;

	while((c = getchar()) != EOF) {
		switch(c) {
		case ' ':
			/* skip ahead to count number of spaces */
			while((c = getchar()) != EOF && c == ' ')
				++space;
			if(TABWIDTH - lastn == space)
			space = (lastn == TABWIDTH) ? TABWIDTH : (TABWIDTH - lastn);
			for(; space > 0; --space)
				putchar(' ');
			lastn = 0;
			space = 0;
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
