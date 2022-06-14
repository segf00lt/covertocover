#include <stdio.h>

/*
 * TODO fix nesting
 * test comment
 * multi-line comment
 */

#define STAR 1
#define SLASH 2

int main(void) {
	int c;
	int state, last;

	state = last = 0;

	while((c = getchar()) != EOF) {
		switch(c) {
		case '*':
			if(last == SLASH) {
				state = 1;
				last = 0;
			} else
				last = STAR;
			break;
		case '/':
			if(last == STAR) {
				state = 0;
				last = 0;
			} else
				last = SLASH;
			break;
		default:
			if(state)
				continue;
			putchar(c);
		}
	}

	return 0;
}
