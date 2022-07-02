#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MINLINES 10
#define MAX 1000

char buf[MAX];
char **lines;

int mygetline(char *buf, int max) {
	int c, n = 0;

	while((c = getchar()) != EOF && c != '\n' && n < (max - 1))
		buf[n++] = c;

	if(c == '\n')
		buf[n++] = c;

	buf[n] = 0;

	return n;
}

int main(int argc, char **argv) {
	int n, i, j;
	char *tmp;

	n = MINLINES;

	if(argc > 1 && !strcmp(argv[1], "-n"))
		n = atoi(argv[2]);

	lines = malloc(n * sizeof(char*));
	for(i = 0; i < n; ++i)
		lines[i] = malloc(MAX);

	for(i = 0; i < n && mygetline(lines[i], MAX) > 0; ++i);

	while(i == n) {
		if(mygetline(buf, MAX) == 0)
			break;

		tmp = *lines;

		for(j = 0; j < i - 1; ++j)
			lines[j] = lines[j + 1];

		lines[j] = tmp;

		strcpy(lines[j], buf);
	}

	for(j = 0; j < i; ++j)
		fputs(lines[j], stdout);

	for(i = 0; i < n; ++i)
		free(lines[i]);
	free(lines);

	return 0;
}
