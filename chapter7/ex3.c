#include <stdio.h>
#include <stdarg.h>

int itob(int n, char a[], int b, int c, int s) {
	int i, j, x, d, h;
	unsigned int ud, un;
	char *p = a;

	i ^= i;
	j ^= j;
	x = b;
	h = (c ? 'A' : 'a');

	if(b != 2 && b != 8 && b != 10 && b != 16)
		return 0;

	if(n < 0 && s) { // if negative and signed is true
		a[i++] = '-';
		p = a + 1;

		if(n == (1 << (sizeof(int) * 8 - 1))) {
			d = -(n % x);
			a[i++] = d + ((d > 9 && b == 16) ? (h - 10) : '0');
			n /= x;
		}

		n = -n;
	}

	if(s) {
		if(b == 16) {
			do {
				d = n % x;
				a[i++] = d + ((d > 9) ? (h - 10) : '0');
			} while((n /= x) > 0);
		} else {
			for(; n > 0; n /= x)
				a[i++] = (n % x) + '0';
		}

		if(*a == '-')
			++j;

	} else {
		un = (unsigned int)n;
		if(b == 16) {
			do {
				ud = un % x;
				a[i++] = ud + ((ud > 9) ? (h - 10) : '0');
			} while((un /= x) > 0);
		} else {
			for(; un > 0; un /= x)
				a[i++] = (un % x) + '0';
		}
	}

	a[i--] = '\0';

	while(j <= i) {
		x = a[i];
		a[i--] = a[j];
		a[j++] = x;
	}

	return 1;
}

void minprintf(char *fmt, ...) {
	va_list ap;
	char convbuf[64];
	char *p, *sval;
	int ival;
	double dval;

	va_start(ap, fmt);
	for(p = fmt; *p; ++p) {
		if(*p != '%') {
			putchar(*p);
			continue;
		}
		switch(*++p) {
		case 'i':
		case 'd':
			ival = va_arg(ap, int);
			itob(ival, convbuf, 10, 0, 1);
			fputs(convbuf, stdout);
			break;
		case 'x':
			ival = va_arg(ap, int);
			itob(ival, convbuf, 16, 0, 0);
			fputs(convbuf, stdout);
			break;
		case 'X':
			ival = va_arg(ap, int);
			itob(ival, convbuf, 16, 1, 0);
			fputs(convbuf, stdout);
			break;
		case 'o':
			ival = va_arg(ap, int);
			itob(ival, convbuf, 8, 0, 0);
			fputs(convbuf, stdout);
			break;
		case 'b':
			ival = va_arg(ap, int);
			itob(ival, convbuf, 2, 0, 0);
			fputs(convbuf, stdout);
			break;
		case 'u':
			ival = va_arg(ap, int);
			itob(ival, convbuf, 10, 0, 1);
			fputs(convbuf, stdout);
			break;
		case 'f':
			dval = va_arg(ap, int);
			printf("%f", dval);
			break;
		case 's':
			sval = va_arg(ap, char *);
			fputs(sval, stdout);
			break;
		default:
			putchar(*p);
			break;
		}
	}

	va_end(ap);
}

int main(void) {
	minprintf("%s = %b\n", "agent", 47);
	return 0;
}
