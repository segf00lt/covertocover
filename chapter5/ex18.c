#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

static inline char* mystrcat(char *s, char *t);
void dcl(void);
void dirdcl(void);
int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

static inline char* mystrcat(char *s, char *t) {
	while(*s++);

	--s;
	while(*s++ = *t++);

	return s; /* avoid schlemiel algorithms */
}

int main(void) {
	while(gettoken() != EOF) {
		strcpy(datatype, token);
		*out = 0;

		dcl();
		if(tokentype != '\n')
			printf("syntax error\n");

		printf("%s:%s %s\n", name, out, datatype);
	}

	return 0;
}

void dcl(void) {
	int ns = 0;
	char *op = out;

	while(gettoken() == '*') ++ns;

	dirdcl();

	while(ns-- > 0)
		op = mystrcat(op, " pointer to");
}

void dirdcl(void) {
	int type;
	char *op = out;

	if(tokentype == '(') {
		dcl();
		
		if(tokentype != ')')
			printf("error: missing )\n");
	} else if(tokentype == NAME)
		strcpy(name, token);
	else
		printf("error: expected name or (dcl)\n");

	while((type = gettoken()) == PARENS || type == BRACKETS) {
		if(type == PARENS)
			op = mystrcat(op, " function returning");
		else {
			op = mystrcat(op, " array of");
			op = mystrcat(op, token);
		}
	}
}

#define ungetchar(c) ungetc(c, stdin)

int gettoken(void) {
	int c;
	char *p = token;

	while((c = getchar()) == ' ' || c == '\t');

	if(c == '(') {
		if((c = getchar()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetchar(c);
			return tokentype = '(';
		}
	} else if(c == '[') {
		for(*p++ = c; (*p++ = getchar()) != ']';);

		*p = 0;

		return tokentype = BRACKETS;
	} else if(isalpha(c)) {
		for(*p++ = c; isalnum(c = getchar());)
			*p++ = c;
		*p = 0;

		ungetchar(c);

		return tokentype = NAME;
	} else
		return tokentype = c;
}
