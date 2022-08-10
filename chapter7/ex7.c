#include <stdio.h>
#include <string.h>
#include <error.h>
#define MAXLINE 1000

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
	FILE *fp;
	char *pattern;
	char line[MAXLINE];
	long lineno = 0;
	int c, except = 0, number = 0, found = 0;

	while(--argc > 0 && (*++argv)[0] == '-') {
		while(c = *++argv[0]) {
			switch(c) {
			case 'x':
				except = 1;
				break;
			case 'n':
				number = 1;
				break;
			default:
				error(1, 0, "illegal option %c\n", c);
			}
		}
	}

	if(argc < 1)
		error(1, 0, "usage: find -x -n -f file pattern\n");

	pattern = *argv++;

	if(argc == 1) {
		fp = stdin;

		while(fgets(line, MAXLINE, fp)) {
			++lineno;

			if((strstr(line, pattern) != NULL) != except) {
				if(number)
					printf("%ld: %s", lineno, line);
				else
					printf("%s", line);
				++found;
			}
		}
		
		return found;
	}

	while(--argc > 0) {
		if((fp = fopen(*argv, "r")) == NULL)
			error(1, 0, "couldn't open file %s", *argv);

		while(fgets(line, MAXLINE, fp)) {
			++lineno;

			if((strstr(line, pattern) != NULL) != except) {
				if(number)
					printf("%ld: %s", lineno, line);
				else
					printf("%s", line);
				++found;
			}
		}
		
		fclose(fp);
		++argv;
	}

	return found;
}
