#include <stdio.h>
#include <error.h>

#define MAX 512
#define PAGE 40

char buf[MAX];
FILE *fp;
int pagecount;
char *title = "<stdin>";

int main(int argc, char **argv) {
	int n;

	if(argc == 1)
		fp = stdin;
	else {
		if((fp = fopen(*++argv, "r")) == 0)
			error(1, 0, "%s: unable to open file", *argv);
		title = *argv;
	}

	do {
		n = 1;
		pagecount = 1;
		printf("## %s ##\n", title);
		while(fgets(buf, MAX, fp)) {
			if(n++ > PAGE) {
				printf("\n-- %i --\n\n## %s ##\n\n", pagecount++, title);
				n = 1;
			}

			fputs(buf, stdout);
		}
	} while(--argc > 1 && (fclose(fp), fp = fopen(*++argv, "r"), title = *argv));

	if(n < PAGE)
		printf("\n-- %i --\n\n", pagecount);

	if(fp != stdin)
		fclose(fp);

	return 0;
}
