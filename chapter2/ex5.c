#include <stdio.h>

char* any(char *s1, char *s2) {
	char *s;
	for(; *s2; ++s2)
		for(s = s1; *s; ++s)
			if(*s == *s2)
				return s;

	return 0;
}

int main(void) {
	char a[] = "helloa";
	char b[] = "world";
	char *p = any(a, b);
	printf("%s\n", p);

	return 0;
}
