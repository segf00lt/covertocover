#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN 1
#define OUT 0

#define INITSIZE 1024

int STATIC_TABLE[INITSIZE];

int cap = INITSIZE;
int *table = STATIC_TABLE;

int main(void) {
	int c, s, n;
	int longest;
	int *tmp;

	longest = 0;
	s = n = 0;
	while((c = getchar()) != EOF) {
		s = (c == ' ' || c == '\n' || c == '\t') ? s + 1 : 0;

		if(s > 1)
			continue;

		if(s == 1) {
			longest = table[n] > longest ? table[n] : longest;
			++n;
		} else
			++table[n];

		if(n >= cap) {
			cap *= 2;
			tmp = calloc(cap, sizeof(int));
			memmove(tmp, table, n * sizeof(int));
			if(table != STATIC_TABLE)
				free(table);
			table = tmp;
		}
	}

	for(int i = 0; longest > 0;) {
		if(table[i] == longest) {
			putchar('|');
			--table[i];
		} else
			putchar(' ');

		if(++i == n) {
			putchar('\n');
			i = 0;
			--longest;
		}
	}

	if(table != STATIC_TABLE)
		free(table);

	return 0;
}
