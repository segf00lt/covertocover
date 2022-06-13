#include <stdio.h>

#define MAXLINE 1000

double mygetline(char s[], int lim) {
	double n;
	int c, i;

	for(i = 0; (i < lim - 1) && ((c = getchar()) != EOF) && (c != '\n'); ++i)
		s[i] = c;

	if(c == '\n') {
		s[i] = c;
		++i;
	}
	s[i] = 0;

	n = (double)i;

	while((c != '\n') && (c != EOF)) {
		c = getchar();
		++n;
	}

	return n;
}

void copy(char *to, char *from) {
	for(int i = 0; (to[i] = from[i]) != 0; ++i);
}

int main(void) {
	double len, max;
	char line[MAXLINE];
	char longest[MAXLINE];

	max = 0;
	while((len = mygetline(line, MAXLINE)) > 0) {
		if(len > max) {
			max = len;
			copy(longest, line);
		}
	}

	if(max > 0)
		printf("%.0f:%s%s", max, longest, (max > (double)MAXLINE) ? "...\n" : "");

	return 0;
}
