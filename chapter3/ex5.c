#include <stdio.h>
#include <string.h>

void reverse(char s[]) {
	int length = strlen(s);
	int c, i, j;

	for (i = 0, j = length - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int itob(int n, char s[], int b) {
	int i = 0, x = b, d;
	char *p = s;

	if(b != 2 && b != 8 && b != 10 && b != 16)
		return 0;

	if(n < 0) {
		s[i++] = '-';
		p = s + 1;

		if(n == (1 << (sizeof(int) * 8 - 1))) {
			d = -(n % x);
			s[i++] = d + ((d > 9 && b == 16) ? 'a' : '0');
			n /= x;
		}

		n = -n;
	}

	if(b == 16) {
		d = n % x;
		for(; n > 0; n /= x)
			s[i++] = d + ((d > 9 && b == 16) ? 'a' : '0');
	} else {
		for(; n > 0; n /= x)
			s[i++] = (n % x) + '0';
	}

	s[i] = '\0';

	reverse(p);

	return 1;
}

int main(void) {
	char s[128];
	itob(5, s, 2);
	printf("%s\n", s);
	return 0;
}
