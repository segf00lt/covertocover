#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char **argv) {
	int c = 0;
	char *prog = argv[0];

	if(!strcmp(prog, "./upper")) {
		while((c = getchar()) != EOF)
			putchar(toupper(c));
	} else if(!strcmp(prog, "./lower")) {
		while((c = getchar()) != EOF)
			putchar(tolower(c));
	} else {
		fprintf(stderr, "error: %s is an invalid command\n", prog);
		return 1;
	}

	return 0;
}
