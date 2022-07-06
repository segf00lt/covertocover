#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINES 5000
#define MAXLEN 1000
char *lineptr[MAXLINES];

// TODO directory order sort
int dircmp(void *a, void *b) {
	char *s1 = *(char **)a;
	char *s2 = *(char **)b;

	while(*s1 && ((*s1 == *s2) || !isalnum(*s1) || !isalnum(*s2))) {
		s1++;
		s2++;
	}

	return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int rdircmp(void *a, void *b) {
	return dircmp(b, a);
}

int dircmpnocase(void *a, void *b) {
	char *s1 = *(char **)a;
	char *s2 = *(char **)b;

	while(*s1 && ((tolower(*s1) == tolower(*s2)) || !isalnum(*s1) || !isalnum(*s2))) {
		s1++;
		s2++;
	}

	return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int rdircmpnocase(void *a, void *b) {
	return dircmpnocase(b, a);
}

int numcmp(void *a, void *b) {
	double v1, v2;

	v1 = v2 = 0.0;

	v1 = atof(*(char **)a);
	v2 = atof(*(char **)b);

	return (int)(v1 - v2);
}

int sortstrcmp(void *a, void *b) {
	char **pp1 = a;
	char **pp2 = b;
	return strcmp(*(char **)a, *(char **)b);
}

int rsortstrcmp(void *a, void *b) {
	return sortstrcmp(b, a);
}

int rnumcmp(char *a, char *b) {
	return numcmp(b, a);
}

int sortstrcmpnocase(void *a, void *b) {
	char *s1 = *(char **)a;
	char *s2 = *(char **)b;

	while(*s1 && (tolower(*s1) == tolower(*s2))) {
		s1++;
		s2++;
	}

	return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int rsortstrcmpnocase(void *a, void *b) {
	return sortstrcmpnocase(b, a);
}

int mygetline(char *buf, int max) {
	int c, n = 0;

	while((c = getchar()) != EOF && c != '\n' && n < (max - 1))
		buf[n++] = c;

	if(c == '\n')
		buf[n++] = c;

	buf[n] = 0;

	return n;
}

int readlines(char *lineptr[], int maxlines) {
	char *p, line[MAXLEN];
	int len, nlines = 0, i = 0;

	while((len = mygetline(line, MAXLEN)) > 0) {
		if(nlines >= maxlines || (p = malloc(len + 1)) == NULL)
			return -1;

		strcpy(p, line);
		lineptr[nlines++] = p;
	}

	return nlines;
}

#define sort_cmp_func_t(id) int (*id)(const void*,const void*)

#define NUMERIC 1
#define REVERSE 2
#define FOLD 4
#define DIRECTORY 8

int main(int argc, char **argv) {
	int i, nlines, flag = 0, ret = 0;
	sort_cmp_func_t(cmp) = (sort_cmp_func_t())sortstrcmp;

	for(i = 1; i < argc; ++i) {
		if(strcmp(argv[i], "-n") == 0) {
			cmp = (sort_cmp_func_t())numcmp;
			flag |= NUMERIC;
		} else if(strcmp(argv[i], "-r") == 0) {
			flag |= REVERSE;
			cmp = (sort_cmp_func_t())rsortstrcmp;
		} else if(strcmp(argv[i], "-f") == 0) {
			flag |= FOLD;
			cmp = (sort_cmp_func_t())sortstrcmpnocase;
		} else if(strcmp(argv[i], "-d") == 0) {
			flag |= DIRECTORY;
			cmp = (sort_cmp_func_t())dircmp;
		}
	}

	if(flag == (NUMERIC | REVERSE))
		cmp = (sort_cmp_func_t())rnumcmp;
	else if(flag == (FOLD | REVERSE))
		cmp = (sort_cmp_func_t())rsortstrcmpnocase;
	else if(flag == (DIRECTORY | REVERSE))
		cmp = (sort_cmp_func_t())rdircmp;
	else if(flag == (DIRECTORY | FOLD))
		cmp = (sort_cmp_func_t())dircmpnocase;
	else if(flag == (DIRECTORY | REVERSE | FOLD))
		cmp = (sort_cmp_func_t())rdircmpnocase;

	if((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		qsort(lineptr, nlines, sizeof(*lineptr), cmp);
		
		for(i = 0; i < nlines; ++i) /* writelines */
			fputs(lineptr[i], stdout);
	} else {
		printf("%s: input too big to sort\n", *argv);
		ret = 1;
	}

	for(i = 0; i < nlines; ++i)
		free(lineptr[i]);

	return ret;
}
