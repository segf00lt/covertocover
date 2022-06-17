#include <stdio.h>

char* itoa(int n, char s[]) {
	int d = n / 10;

	if(d != 0) {
		if(d < 0) {
			*(s++) = '-';
			d = -d;
		}
		s = itoa(d, s);
	}

	d = (n % 10);
	if(d < 0)
		*(s++) = -d + '0';
	else
		*(s++) = d + '0';

	*s = '\0';
	return s;
}

int main(void) {
	char s[128];
	itoa(1<<(sizeof(int)*8-1), s);
	printf("%s\n", s);
	return 0;
}
