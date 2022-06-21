#include <stdio.h>

void mystrncpy(char *s, char *t, int n) {
	while(n-- > 0 && (*s++ = *t++));
	*s = 0;
}

char* mystrncat(char *s, char*t, int n) {
	while(*s++);
	s--;

	while(n-- > 0 && (*s++ = *t++));

	*s = 0;

	return s;
}

int mystrncmp(char *s, char *t, int n) {
	while(n-- > 0 && *s && *t)
		if(*s++ != *t++)
			return 0;

	return 1;
}

int main(void) {
	char a[20];
	char b[] = "hello world";
	mystrncpy(a, b, 5);
	printf("%s\n", a);
	char c[] = " world!!!";
	mystrncat(a, c, 7);
	printf("%s\n", a);
	if(mystrncmp(a, b, 11))
		printf("yes\n");
	return 0;
}
