#include <stdio.h>

void squeeze(char *s1, char *s2) {
	int r1 = 0, w1 = 0, r2;

	for(; s1[r1]; ++r1) {
		s1[w1] = s1[r1];
		for(r2 = 0; s2[r2]; ++r2)
			if(s1[r1] == s2[r2])
				break;
		if(s2[r2] == 0)
			++w1;
	}

	s1[w1] = 0;
}

int main(void) {
	char a[] = "helloa";
	char b[] = "world";
	squeeze(a, b);
	printf("%s\n", a);

	return 0;
}
