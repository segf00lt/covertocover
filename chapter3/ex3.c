#include <stdio.h>

static inline int alphanum(int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

void expand(char s1[], char s2[]) {
	int p = 0, n = 0, c;

	for(; *s1; ++s1) {
		n = *(s1 + 1);

		if(*s1 == '-' && alphanum(p) && alphanum(n) && p < n) {
			for(c = p + 1; c <= n; ++c)
				*(s2++) = c;
			++s1;
			p = 0;
		} else {
			*(s2++) = *s1;
			p = *s1;
		}
	}

	*s2 = '\0';
}

int main(void) {
	char s[128];
	char t[] = "a-f-g0-9ABC-0";
	expand(t, s);
	printf("%s\n", s);

	return 0;
}
