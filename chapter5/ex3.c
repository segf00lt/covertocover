#include <stdio.h>
#include <ctype.h>

char* strcat(char *s, char *t) {
	while(*s++);
	--s;
	while(*s++ = *t++);

	return s;
}

int main(void) {
	char s[26] = "abcde";
	char t[] = "fghijklmno";
	strcat(s, t);
	printf("%s\n", s);
	return 0;
}
