#include <stdio.h>

/* count whitespace */

int main(void) {
	int c;
	double n;

	for(n = 0; (c = getchar()) != EOF;) {
		switch(c) {
		case '\t':
		case '\n':
		case ' ':
			++n;
		}
	}
	printf("%.0f\n", n);

	return 0;
}
