#include <stdio.h>

// 85 characters
//8888888888888888888888888888888888888888888888888888888888888888888888888888888888
// 93 characters
//888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888
// 79 characters
//8888888888888888888888888888888888888888888888888888888888888888888888888888

char buf[81];

int main(void) {
	int i, c;
	buf[80] = i = 0;

	while((c = getchar()) != EOF) {
		if(i < 79) {
			buf[i++] = c;
			if(c == '\n')
				i = 0;
		} else {
			i = 0;
			printf("%s", buf);
			for(; c != EOF && c != '\n'; c = getchar())
				putchar(c);
			putchar('\n');
		}
	}

	return 0;
}
