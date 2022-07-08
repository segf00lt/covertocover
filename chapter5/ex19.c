#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define STRLEN(x) (sizeof(x) / sizeof(*x)) - 1 /* compile-time strlen */
#define MAXLEN 1000
#define MAXTOKEN 100

static inline char* mystrcat(char *s, char *t);
int mygetline(char *buf, int max);
int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char line[MAXLEN];
char out[1000];
char *p;
char func[] = "function returning";
char point[] = "pointer to";
char array[] = "array of";

enum { NAME, PARENS, BRACKETS, STAR, ERR, END };

int main(void) {
	char tmp[MAXTOKEN];
	int linelen;
	p = line;

	while((linelen = mygetline(line, MAXLEN)) > 0) {
		line[linelen - 1] = 0;
		*(p = strchr(p, ':')) = 0;
		strcpy(out, line); /* set variable name */
		++p;
		
		for(gettoken(); tokentype != END;) {
			if(tokentype == PARENS || tokentype == BRACKETS)
				mystrcat(out, token);
			else if(tokentype == STAR) {
				gettoken(); /* lookahead */

				if(tokentype == PARENS || tokentype == BRACKETS)
					sprintf(tmp, "(*%s)", out);
				else
					sprintf(tmp, "*%s", out);

				strcpy(out, tmp);
				continue;
			} else if(tokentype == NAME) {
				sprintf(tmp, "%s %s", token, out);
				strcpy(out, tmp);
			} else if(tokentype == ERR) {
				printf("error: invalid input at: %s\n", p);
				exit(1);
			}

			gettoken();
		}
		puts(out);
	}

	return 0;
}

int gettoken(void) {
	char *s;
	while(*p == ' ' || *p == '\t') ++p;

	if(strstr(p, func) == p) {
		p += STRLEN(func);
		strcpy(token, "()");
		return tokentype = PARENS;
	} else if(strstr(p, point) == p) {
		p += STRLEN(point);
		return tokentype = STAR;
	} else if(strstr(p, array) == p) {
		p += STRLEN(array);
		strcpy(token, "[]");
		return tokentype = BRACKETS;
	} else if(*p == '\n' || *p == 0) {
		return tokentype = END;
	} else {
		for(s = p; *s; ++s)
			if(!isalnum(*s))
				return tokentype = ERR;

		strcpy(token, p);
		p = s;
		return tokentype = NAME;
	}
}

static inline char* mystrcat(char *s, char *t) {
	while(*s) ++s;

	while(*s++ = *t++);

	return s - 1; /* avoid schlemiel algorithms */
}

int mygetline(char *buf, int max) {
	int c, n = 0;

	while((c = getchar()) != EOF && c != '\n' && n < (max - 1))
		buf[n++] = c;

	if(c == '\n')
		buf[n++] = c;

	buf[n] = 0;

	return n;
}
