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
	int i, j, c;

	while((l = mygetline(buf, MAXLINE)) > 0) {
		for(i = 0, j = l - 2; i < j; ++i, --j) {
			c = buf[i];
			buf[i] = buf[j];
			buf[j] = c;
		}

		printf("%s", buf);
	}

	return 0;
}
