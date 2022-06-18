#include <stdio.h>

void reverse(char s[], char *e) {
	int c;

	c = *s;
	*s = *e;
	*e = c;

	if(e <= s)
		return;

	reverse(++s, --e);
}

int main(void) {
	char s[] = "hello world";
	char *e = s + 10;
	reverse(s, e);
	printf("%s\n", s);
	return 0;
}
