#include <stdio.h>

int strend(char *s, char*t) {
	int i, j;

	for(i = 0; *s; ++s, ++i);
	for(j = 0; *t; ++t, ++j);

	for(--i, --j; i >= 0 && j >= 0; --i, --j)
		if(*--s != *--t)
			break;

	return j < 0;
}

int main(void) {
	char s[] = "hello world";
	char t[] = "world";
	if(strend(s, t))
		printf("yes\n");
	return 0;
}
