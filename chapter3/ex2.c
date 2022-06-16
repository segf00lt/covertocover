#include <stdio.h>

void escape(char s[], char t[]) {
	for(; *t; ++t) {
		switch(*t) {
		case '\t':
			*(s++) = '\\';
			*(s++) = 't';
			break;
		case '\n':
			*(s++) = '\\';
			*(s++) = 'n';
			break;
		default:
			*(s++) = *t;
		}
	}

	*s = *t;
}

void unescape(char s[], char t[]) {
	for(; *t; ++t) {
		if(t[0] == '\\' && t[1] == 't') {
			*(s++) = '\t';
			++t;
		} else if(t[0] == '\\' && t[1] == 'n') {
			*(s++) = '\n';
			++t;
		} else
			*(s++) = *t;
	}

	*s = *t;
}

int main(void) {
	char s[20];
	char y[20];
	char t[] = "hello\tworld";
	escape(s, t);
	printf("%s\n", s);
	unescape(y, s);
	printf("%s\n", y);

	return 0;
}
