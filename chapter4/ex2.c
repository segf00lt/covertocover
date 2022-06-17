#include <stdio.h>
#include <ctype.h>

double myatof(char s[]) {
	double val, power, e;
	int i, sign;

	for(i = 0; isspace(s[i]); ++i);

	sign = (s[i] == '-') ? -1 : 1;

	if(s[i] == '-' || s[i] == '+')
		++i;

	for(val = 0.0; isdigit(s[i]); ++i)
		val = 10.0 * val + (s[i] - '0');

	if(s[i] == '.')
		++i;

	for(power = 1.0; isdigit(s[i]); power *= 10.0, ++i)
		val = 10.0 * val + (s[i] - '0');

	val = sign * (val / power);

	if(s[i] == 'e' || s[i] == 'E')
		++i;
	else
		return val;

	sign = (s[i] == '-') ? -1 : 1;

	if(s[i] == '-' || s[i] == '+')
		++i;

	for(e = 0.0; isdigit(s[i]); ++i)
		e = 10.0 * e + (s[i] - '0');

	for(power = 1.0; e > 0.0; power *= 10.0, e -= 1.0);

	val = (sign < 0) ? (val / power) : (val * power);

	return val;
}

int main(void) {
	printf("%g\n", myatof("12.2E-3"));
	return 0;
}
