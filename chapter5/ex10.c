/* reverse-polish expr */

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

int nargs;
double stack[MAX];
int sp = -1;
double popstack[2];
int psp = -1;
double vars[26];
int v = 0;;
double f, op2;
int t;
int op;

int gettoken(char **s) {
	static int i = 1;
	char *e;

	if(i == nargs)
		return END;

	for(e = s[i]; *e; ++e);
	f = strtod(s[i], &e);

	if(e != s[i]) {
		op = NONE;
		++i;
		return NUM;
	}

	switch(*s[i]) {
	case '=': case '+': case '-': case '*': case '/':
	case '%': case '^':
		op = *s[i];
		++i;
		return OP;
	}

	if(!strcmp(s[i], "sin")) {
		++i;
		op = SIN;
		return OP;
	} else if(!strcmp(s[i], "cos")) {
		++i;
		op = COS;
		return OP;
	} else if(!strcmp(s[i], "exp")) {
		++i;
		op = EXP;
		return OP;
	} else if(!strcmp(s[i], "sqrt")) {
		++i;
		op = SQRT;
		return OP;
	} else if(!strcmp(s[i], "top")) {
		++i;
		op = TOP;
		return OP;
	} else if(!strcmp(s[i], "swap")) {
		++i;
		op = SWAP;
		return OP;
	} else if(!strcmp(s[i], "all")) {
		++i;
		op = ALL;
		return OP;
	} else if(!strcmp(s[i], "clear")) {
		++i;
		op = CLEAR;
		return OP;
	} else if(!strcmp(s[i], "pop")) {
		++i;
		op = POP;
		return OP;
	} else if(*s[i] >= 'a' && *s[i] <= 'z') {
		op = NONE;
		v = *s[i++] - 'a';

		if(*s[i + 1] == '=')
			return VAR;

		f = vars[v];

		return NUM;
	}

	printf("error: unkown token\n\t%s\t^^^\n", s[i]);

	op = NONE;
	return ERR;
}

int main(int argc, char **argv) {
	if(argc == 1) {
		printf("error: missing arguments\n");
		return 1;
	}

	nargs = argc;

	for(t = gettoken(argv); t != END && t != ERR; t = gettoken(argv)) {
		if(t == NUM)
			stack[++sp] = f;

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

	if(t == ERR)
		return 1;

	printf("%g\n", stack[sp]);

	return 0;
}
