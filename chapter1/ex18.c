#include <stdio.h>

#define MAXLINE 1000

char buf[MAXLINE];

int mygetline(char s[], int max) {
	int c, n = 0;

	while((c = getchar()) != EOF && c != '\n' && n < (max - 1))
		s[n++] = c;

	if(c == '\n')
		s[n++] = c;

	s[n] = 0;

	return n;
}

int main(void) {
	int l;
	char *p;

	while((l = mygetline(buf, MAXLINE)) > 0) {
		p = buf + l - 1;

		for(; (p - 1) >= buf; --p) {
			switch(*(p - 1)) {
			case ' ':
			case '\t':
				continue;
			default:
				*p = 0;
			}

			break;
		}

		if(*p == '\n')
			continue;

		printf("%s\n", buf);
	}

	return 0;
}
