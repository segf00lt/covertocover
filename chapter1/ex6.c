#include <stdio.h>

int main(void) {
	int c;
	int b;

	while(b = ((c = getchar()) != EOF))
		if(b == 1)
			printf("expression is 1\n");

	printf("expression is 0\n");

	return 0;
}
