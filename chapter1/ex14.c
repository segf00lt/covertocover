#include <stdio.h>
#include <string.h>

#define LENGTH 128
#define PRINTABLE 32

double hist[LENGTH];
double top;

int main(void) {
	top = 0;
	memset(hist, 0, LENGTH);

	for(int c; (c = getchar()) != EOF;) {
		++hist[c];
		if(hist[c] > top)
			top = hist[c];
	}

	int i;
	for(i = 0; top > 0;) {
		if(hist[i] == top) {
			putchar('|');
			--hist[i];
		} else
			putchar(' ');

		putchar(' ');

		if(++i >= LENGTH) {
			putchar('\n');
			i = 0;
			--top;
		}
	}

	for(i = 0; i < PRINTABLE; ++i)
		printf("%i ", i);

	for(; i < LENGTH; ++i)
		printf("%c ", i);

	putchar('\n');

	return 0;
}
