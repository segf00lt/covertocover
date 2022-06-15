#include <stdio.h>

int htoi(char *s) {
	char *p;
	int power16 = 1;
	int i = 0;

	for(p = s; *p; ++p, power16 *= 16);
	power16 /= 16;

	if(p-s >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
		s += 2;
		power16 /= 256;
	}

	for(p = s; *p; ++p, power16 /= 16) {
		if(*p >= 'a' && *p <= 'f')
			i += (10 + *p - 'a') * power16;
		else if(*p >= 'A' && *p <= 'F')
			i += (10 + *p - 'A') * power16;
		else if(*p >= '0' && *p <= '9')
			i += (*p - '0') * power16;
		else
			return 0;
	}

	return i;
}

int main(void) {
	printf("%i\n", htoi("0xa"));

	return 0;
}
