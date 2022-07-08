#include <stdio.h>
#include <ctype.h>

#define ungetchar(c) ungetc(c, stdin)

int iswordchar(int c) {
	return 
		(c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
		 c == '_' || c == '"'  ||  c == '\''|| c == '~' ||
		 c == '#' || c == '$'  ||  c == '&' || c == '*' ||
		 c == '(' || c == ')'  ||  c == '[' || c == ']';
}

int getword(char *word, int lim) {
	int c;
	char *w = word;

	while(isspace(c = getchar()));

	if(c != EOF)
		*w++ = c;

	if(!iswordchar(c)) {
		*w = 0;
		return c;
	}

	for(; --lim > 0; ++w)
		if(!iswordchar(*w = getchar())) {
			ungetchar(*w);
			break;
		}

	*w = 0;

	return word[0];
}

int main(void) {
	char buf[1000];
	while(getword(buf, 1000) != EOF)
		puts(buf);
	return 0;
}
