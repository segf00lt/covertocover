#include <stdio.h>

int main(void) {
	double x0, x1, x2;
	char op;
	unsigned int error, n;

	while((n = scanf("%lf %lf %c", &x1, &x2, &op)) != EOF) {
		error ^= error;

		switch(op) {
		case '+':
			x0 = x1 + x2;
			break;
		case '*':
			x0 = x1 * x2;
			break;
		case '-':
			x0 = x1 - x2;
			break;
		case '/':
			if(x2 != 0.0)
				x0 = x1 / x2;
			else
				error = ~error;
			break;
		default:
			printf("error: bad command\n");
			continue;
		}

		if(error)
			printf("error: zero division\n");
		else
			printf("==> %.8g\n", x0);
	}
}


