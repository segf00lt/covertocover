#include <stdio.h>
#include <stdarg.h>
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

int getdouble(double *pn) {
	int c;
	double sign, power, e;

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

int minscanf(char *fmt, ...) {
	va_list ap;
	char *p;
	int c, count;
	char *saddr;
	int *iaddr;
	double *daddr;

	count ^= count;

	va_start(ap, fmt);
	for(p = fmt; *p; ++p) {
		if(*p != '%') {
			if((c = getchar()) != *p)
				return count;
		}

		switch(*++p) {
		case 's':
			for(saddr = va_arg(ap, char *); (c = getchar()) != p[1]; *saddr++ = c);

			++count;
			break;
		case 'f':
			daddr = va_arg(ap, double *);
			getdouble(daddr);
			++count;
			break;
		case 'd':
			iaddr = va_arg(ap, int *);
			getint(iaddr);
			++count;
			break;
		}
	}

	va_end(ap);
}

int main(void) {
	int i = 0;
	minscanf("%d", &i);
	printf("%i\n", i);
	return 0;
}
