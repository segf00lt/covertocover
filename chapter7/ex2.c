#include <stdio.h>

#define LINELEN 30
//#define HEX
#define OCT

int main(void) {
	int c, n;

	for(n = 1; (c = getchar()) != EOF; ++n) {
		if(c == '\n' || n >= LINELEN) {
			n = 1;
			if(c != '\n')
				putchar('\n');
		}

		if(c < ' ' && c != '\n')
#if defined(HEX)
			printf("\\0x%x", c);
#elif defined(OCT)
			printf("\\0%o", c);
#endif
		else
			putchar(c);
	}

	return 0;
}
