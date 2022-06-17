#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXLEN 100
#define NUMBER 0
#define ZERODIV 1

int getch(void);
void ungetch(int c);
int getop(char s[]);
void push(double f);
double pop(void);

char buf[MAXLEN];
int bufp = 0;

int sp = 0;
double val[MAXLEN];

int getch(void) {
	return (bufp > 0) ? buf[--bufp] : getchar();
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

double pop(void) {
	if(sp > 0)
		return val[--sp];

	printf("error: stack empty\n");
	return 0.0;
}

int main(void) {
	int type;
	double op2;
	char s[MAXLEN];
	int error = 0;

	while((type = getop(s)) != EOF) {
		switch(type) {
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
		case '/':
			op2 = pop();
			if(op2 != 0.0)
				push(pop() / op2);
			else
				error = ZERODIV;
			break;
		case '%':
			op2 = pop();
			if(op2 != 0.0)
				push(fmod(pop(), op2));
			else
				error = ZERODIV;
			break;
		case '\n':
			switch(error) {
			case 0:
				printf("=>  %.8g\n", pop());
				break;
			case ZERODIV:
				printf("error: zero divisor\n");
				break;
			}

			error = 0;
			break;
		default:
			printf("error: unkown command\n");
			break;
		}
	}
}


