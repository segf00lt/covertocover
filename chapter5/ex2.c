#include <stdio.h>
#include <ctype.h>

int getfloat(float *pn) {
	int c;
	float sign, power, e;

	while(isspace(c = getc(stdin)));

	if(!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetc(c, stdin);
		return 0;
	}

	sign = (c == '-') ? -1.0 : 1.0;

	if(c == '+' || c == '-')
		c = getc(stdin);

	if(!isdigit(c)) {
		ungetc(c, stdin);
		return 0;
	}

	for(*pn = 0.0; isdigit(c); c = getc(stdin))
		*pn = 10.0 * *pn + (c - '0');

	if(c == '.')
		c = getc(stdin);

	for(power = 1.0; isdigit(c); power *= 10.0, c = getc(stdin))
		*pn = 10.0 * *pn + (c - '0');

	*pn = sign * (*pn / power);

	if(c == 'e' || c == 'E')
		c = getc(stdin);
	else
		return c;

	sign = (c == '-') ? -1.0 : 1.0;

	if(c == '+' || c == '-')
		c = getc(stdin);

	for(e = 0.0; isdigit(c); c = getc(stdin))
		e = 10.0 * e + (c - '0');

	for(power = 1.0; e > 0.0; power *= 10.0, e -= 1.0);

	*pn = (sign < 0) ? (*pn / power) : (*pn *power);

	return c;
}


int main(void) {
	float f = 0;
	if(getfloat(&f))
		printf("%g\n", f);
	return 0;
}
