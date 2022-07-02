#include <stdio.h>
#include <string.h>

#define MAXLEN 1000
#define MAXLINES 100

char memory[MAXLINES][MAXLEN];
char *lines[MAXLINES];

int mygetline(char *buf, int max) {
	int c, n = 0;

	while((c = getchar()) != EOF && c != '\n' && n < (max - 1))
		buf[n++] = c;

	if(c == '\n')
		buf[n++] = c;

	buf[n] = 0;

	return n;
}

int readlines(char *lineptr[], int maxlines, char mem[][MAXLEN]) {
	char *p, line[MAXLEN];
	int len, nlines = 0, i = 0;

	while((len = mygetline(line, MAXLEN)) > 0) {
		if(nlines >= maxlines)
			return -1;

		strcpy((p = mem[i++]), line);
		lineptr[nlines++] = p;
	}

	return nlines;
}

int main(void) {
	int n;

	if((n = readlines(lines, MAXLINES, memory)) < 0) {
		printf("ERROR\n");
		return 1;
	}

	for(int i = 0; i < n; ++i)
		fputs(lines[i], stdout);

	return 0;
}
