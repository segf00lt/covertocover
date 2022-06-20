/* reverse polish calculator
 *
 * this design incorporates all the features from exercises 3-10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 512

enum {
	/* tokens */
	NUM,
	VAR,
	OP,
	END,
	ERR,

	/* special op values */
	NONE = 0,
	SIN = 1,
	COS = 2,
	EXP = 3,
	SQRT = 4,
	TOP,
	SWAP,
	ALL,
	CLEAR,
	POP,
};

char buf[MAX];
int len;
double stack[MAX];
int sp = -1;
double popstack[2];
int psp = -1;
double vars[26];
int v = 0;;
double f, op2;
int t;
int op;

int mygetline(char *buf, int max) {
	int c, n = 0;

	while((c = getchar()) != EOF && c != '\n' && n < (max - 1))
		buf[n++] = c;

	if(c == '\n')
		buf[n++] = c;

	buf[n] = 0;

	return n;
}

int gettoken(void) {
	static char *s = buf;
	static char *e;
	char *tmp;

	e = buf + len;

	f = strtod(s, &e);

	if(e != s) {
		op = NONE;
		s = e;
		return NUM;
	}

	while(*s == ' ' || *s == '\t') ++s;

	switch(*s) {
	case '\n': case '\0':
		s = buf;
		return END;
	case '=': case '+': case '-': case '*': case '/':
	case '%': case '^':
		op = *(s++);
		return OP;
	}

	if(strstr(s, "sin") == s) {
		s += 3;
		op = SIN;
		return OP;
	} else if(strstr(s, "cos") == s) {
		s += 3;
		op = COS;
		return OP;
	} else if(strstr(s, "exp") == s) {
		s += 3;
		op = EXP;
		return OP;
	} else if(strstr(s, "sqrt") == s) {
		s += 4;
		op = SQRT;
		return OP;
	} else if(strstr(s, "top") == s) {
		s += 3;
		op = TOP;
		return OP;
	} else if(strstr(s, "swap") == s) {
		s += 4;
		op = SWAP;
		return OP;
	} else if(strstr(s, "all") == s) {
		s += 3;
		op = ALL;
		return OP;
	} else if(strstr(s, "clear") == s) {
		s += 5;
		op = CLEAR;
		return OP;
	} else if(strstr(s, "pop") == s) {
		s += 3;
		op = POP;
		return OP;
	} else if(*s >= 'a' && *s <= 'z') {
		op = NONE;
		v = *s - 'a';
		tmp = ++s;

		/* lookahead 2 tokens */
		while(*tmp == ' ' || *tmp == '\t' || *tmp == '\n') ++tmp;
		while(*tmp != ' ' && *tmp != '\t' && *tmp != '\n') ++tmp;
		while(*tmp == ' ' || *tmp == '\t' || *tmp == '\n') ++tmp;

		if(*tmp == '=')
			return VAR;

		f = vars[v];

		return NUM;
	}

	printf("error: unkown token\n\t%s\t^^^\n", s);

	op = NONE;
	return ERR;
}

int main(void) {
	while((len = mygetline(buf, MAX)) > 0) {
		for(t = gettoken(); t != END && t != ERR; t = gettoken())
			if(t == NUM)
				stack[++sp] = f;

		if(t == ERR)
			continue;

		if(sp < 0) {
			printf("error: stack empty\n");
			continue;
		}

		switch(op) {
		case TOP: /* print top element */
			printf("==>\t%g\n", stack[sp]);
			break;
		case SWAP: /* swap 2 top elements */
			if(sp < 1) {
				printf("error: not enough elemnts in stack\n");
				break;
			}

			op2 = stack[sp--];
			f = stack[sp--];
			stack[++sp] = op2;
			stack[++sp] = f;
			break;
		case ALL: /* print all elemnts */
			printf("stack:\n");
			for(int i = sp; i >= 0; --i)
				printf("==>\t%g\n", stack[i]);
			break;
		case CLEAR: /* clear stack */
			while(sp >= 0) stack[sp--] = 0.0;
			break;
		case POP:
			stack[sp--] = 0.0;
			break;
		case '=': /* assign to var */
			vars[v] = stack[sp--];
			break;
		case '+':
			if(sp < 1) {
				printf("error: + requires 2 arguments\n");
				sp = -1;
			} else {
				op2 = stack[sp--];
				f = stack[sp--];
				stack[++sp] = f + op2;
			}
			break;
		case '-':
			if(sp < 1) {
				printf("error: - requires 2 arguments\n");
				sp = -1;
			} else {
				op2 = stack[sp--];
				f = stack[sp--];
				stack[++sp] = f - op2;
			}
			break;
		case '*':
			if(sp < 1) {
				printf("error: * requires 2 arguments\n");
				sp = -1;
			} else {
				op2 = stack[sp--];
				f = stack[sp--];
				stack[++sp] = f * op2;
			}
			break;
		case '/':
			if(sp < 1) {
				printf("error: / requires 2 arguments\n");
				sp = -1;
				break;
			}

			op2 = stack[sp--];
			if(op2 == 0) {
				printf("error: zero divisor\n");
				sp = -1;
				break;
			}
			f = stack[sp--];
			stack[++sp] = f / op2;
			break;
		case '%':
			if(sp < 1) {
				printf("error: % requires 2 arguments\n");
				sp = -1;
				break;
			}

			op2 = stack[sp--];
			if(op2 == 0) {
				printf("error: zero divisor\n");
				break;
			}
			f = stack[sp--];
			stack[++sp] = fmod(f, op2);
			break;
		case '^':
			if(sp < 1) {
				printf("error: ^ requires 2 arguments\n");
				sp = -1;
				break;
			}

			op2 = stack[sp--];
			f = stack[sp--];
			if((f == 0 && op2 <= 0) || (f < 0 && floor(op2) != op2)) {
				printf("error: domain error\n");
			} else
				stack[++sp] = pow(f, op2);
			break;
		case SIN:
			f = stack[sp--];
			stack[++sp] = sin(f);
			break;
		case COS:
			f = stack[sp--];
			stack[++sp] = cos(f);
			break;
		case EXP:
			f = stack[sp--];
			stack[++sp] = exp(f);
			break;
		case SQRT:
			f = stack[sp--];
			if(f < 0)
				printf("error: domain error\n");
			else
				stack[++sp] = sqrt(f);
		}

	}

	return 0;
}
