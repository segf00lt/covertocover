#include <stdio.h>

int strrindex(char s[], char t[]) {
	char *s1, *s2, *r;

	for(s1 = s; *s1; ++s1)
		for(s2 = s1, r = t; *r; ++s2, ++r)
			if(r[0] == *s2 && r[1] == '\0')
				return s1 - s;
	return -1;
}

int main(void) {
	char s[] = "nibble";
	char t[] = "bbl";
	printf("%i\n", strrindex(s, t));
	return 0;
}
