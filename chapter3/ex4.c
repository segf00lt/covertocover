#include <stdio.h>
#include <string.h>
#include <limits.h>

void reverse(char s[]) {
	int length = strlen(s);
	int c, i, j;

	for (i = 0, j = length - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void itoa(int n, char s[]) {
	int i = 0;
	char *p = s;

	if(n < 0) {
		s[i++] = '-';
		p = s + 1;

		if(n == (1 << (sizeof(int) * 8 - 1))) {
			s[i++] = -(n % 10) + '0';
			n /= 10;
		}

		n = -n;
	}

	for(; n > 0; n /= 10)
		s[i++] = (n % 10) + '0';

	s[i] = '\0';

	reverse(p);
}

int main(void) {
	char s[128];
	itoa(1<<((sizeof(int)*8)-1), s);
	printf("%s\n", s);
	return 0;
}
