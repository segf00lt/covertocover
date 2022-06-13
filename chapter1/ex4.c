#include <stdio.h>

int main(void) {
	float celsius, fahr;

	printf("celsius\tfahrenheit\n");
	for(celsius = 0; celsius <= 300; celsius += 20) {
		fahr = ((9.0 * celsius) / 5.0) + 32.0;
		printf("%3.0f\t%6.1f\n", celsius, fahr);
	}

	return 0;
}
