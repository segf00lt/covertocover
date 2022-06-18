#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXLEN 100
#define NUMBER 0
#define ZERODIV 1
#define DOMAINERR 2

int getch(void);
void ungetch(int c);
int getop(char s[]);
void push(double f);
int pop(double *f);

char buf[MAXLEN];
int bufp = 0;

int sp = 0;
double val[MAXLEN];

int getch(void) {
	int c = getchar();

	if(c == EOF)
		exit(0);

	return (bufp > 0) ? buf[--bufp] : c;
}

void ungetch(int c) {
	if(bufp >= MAXLEN)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

int getop(char s[]) {
	int i, c;

	while((*s = c = getch()) == ' '|| c == '\t');
	s[1] = '\0';

	if(!isdigit(c) && c != '.')
		return c;

	i = 0;
	if(isdigit(c))
		while(isdigit(s[++i] = c = getch()));
	if(c == '.')                               
		while(isdigit(s[++i] = c = getch()));
	s[i] = '\0';

	if(c != EOF)
		ungetch(c);

	return NUMBER;
}

void push(double f) {
	if(sp < MAXLEN)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

int pop(double *f) {
	if(sp > 0) {
		*f = val[--sp];
		return 1;
	}

	printf("error: stack empty\n");
	return 0;
}

int main(void) {
	int type;
	double op2, f;
	char s[MAXLEN];
	int error = 0;

	while((type = getop(s)) != EOF) {
		error = 0;

		switch(type) {
		case NUMBER:
			push(atof(s));
			break;
		case '_': /* peek */
			if(pop(&f)) {
				printf("=>  %.8g\n", f);
				push(f);
			}
			break;
		case '@': /* swap */
			pop(&op2);
			pop(&f);
			push(op2);
			push(f);
			break;
		case '#': /* clear */
			while(sp > 0) --sp;
			break;
		case 'S':
			pop(&f);
			push(sin(f));
			break;
		case 'E':
			pop(&f);
			push(exp(f));
			break;
		case '^':
			pop(&op2);
			pop(&f);
			if((f == 0 && op2 <= 0) || (f < 0 && op2 != floor(op2)))
				error = DOMAINERR;
			else
				push(pow(f, op2));
			break;
		case '+':
			pop(&op2);
			pop(&f);
			push(f + op2);
			break;
		case '*':
			pop(&op2);
			pop(&f);
			push(f * op2);
			break;
		case '-':
			pop(&op2);
			pop(&f);
			push(f - op2);
		case '/':
			pop(&op2);
			pop(&f);
			if(op2 != 0.0)
				push(f / op2);
			else
				error = ZERODIV;
			break;
		case '%':
			pop(&op2);
			pop(&f);
			if(op2 != 0.0)
				push(fmod(f, op2));
			else
				error = ZERODIV;
			break;
		case '\n':
			switch(error) {
			case ZERODIV:
				printf("error: zero divisor\n");
				break;
			case DOMAINERR:
				printf("error: domain error\n");
				break;
			}
			break;
		default:
			printf("error: unkown command\n");
			break;
		}
	}
}


