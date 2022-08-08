#include <stdio.h>
#include <error.h>
#include <string.h>

#define MAX 512

char buf1[MAX], buf2[MAX];
FILE *fp1, *fp2;

int main(int argc, char **argv) {
	if(argc < 3)
		error(1, 0, "not enough arguments");

	if((fp1 = fopen(argv[1], "r")) == 0)
		error(1, 0, "%s: unable to open file", argv[1]);
	if((fp2 = fopen(argv[2], "r")) == 0)
		error(1, 0, "%s: unable to open file", argv[2]);

	while(fgets(buf1, MAX, fp1) > 0 && fgets(buf2, MAX, fp2) > 0)
		if(strcmp(buf1, buf2))
			printf("%s: %s%s: %s", argv[1], buf1, argv[2], buf2);

	fclose(fp1);
	fclose(fp2);

	return 0;
}
