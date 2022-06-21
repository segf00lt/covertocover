#include <stdio.h>
#include <ctype.h>

int getint(int *pn) {
	int c, sign;
	
	while(isspace(c = getc(stdin)));

	if(!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetc(c, stdin);
		return 0;
	}

	sign = (c == '-') ? -1 : 1;

	if(c == '+' || c == '-')
		c = getc(stdin);

	if(!isdigit(c)) {
		ungetc(c, stdin);
		return 0;
	}

	for(*pn = 0; isdigit(c); c = getc(stdin))
		*pn = 10 * *pn + (c - '0');

	*pn *= sign;

	if(c != EOF)
		ungetc(c, stdin);

	return c;
}

int main(void) {
	int i = 0;
	if(getint(&i))
		printf("%i\n", i);
	return 0;
}
