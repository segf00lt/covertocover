#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ungetchar(c) ungetc(c, stdin)

typedef struct V_node V_node;
struct V_node {
	char *name;
	V_node *left;
	V_node *right;
};

typedef struct S_node S_node;
struct S_node {
	char *suffix;
	V_node *vartree;
	S_node *left;
	S_node *right;
};

enum { NAME, STAR, ARR, RBRACE, LBRACE, SEMI, EQ, OTHER, END };

void printsuffix(S_node *sroot);
void printvar(V_node *vroot);
void savevar(char *name);
int gettoken(void);
int getvarname(void);

char namebuf[512];
S_node *suffixtree;
int tokentype;
int lastwasstruct;
int suffixlen;

int main(int argc, char **argv) {
	char *p;

	suffixlen = 6;

	if(argc > 3) {
		fprintf(stderr, "error: too many arguments\n");
		exit(1);
	}

	if(argc == 3 && !strcmp(argv[1], "-n"))
		suffixlen = atoi(argv[2]);

	getvarname(); // handle first node
	suffixtree = calloc(1, sizeof(S_node));
	suffixtree->vartree = calloc(1, sizeof(V_node));
	suffixtree->suffix = strndup(namebuf, suffixlen);
	suffixtree->vartree->name = strdup(namebuf);

	while(getvarname()) {
		p = strdup(namebuf);
		savevar(p);
	}

	printsuffix(suffixtree);

	return 0;
}

void printsuffix(S_node *sroot) {
	if(sroot) {
		printsuffix(sroot->left);
		printvar(sroot->vartree);
		printsuffix(sroot->right);
		free(sroot->suffix);
		free(sroot);
	}
}

void printvar(V_node *vroot) {
	if(vroot) {
		printvar(vroot->left);
		puts(vroot->name);
		printvar(vroot->right);
		free(vroot->name);
		free(vroot);
	}
}

void savevar(char *name) {
	char *s;
	int compare, exists;
	S_node *sroot, *sp;
	V_node *vroot, *vp;

	compare = exists = 0;
	s = strndup(name, suffixlen);

	sroot = suffixtree;
	while(1) {
		if(((compare = strcmp(sroot->suffix, s)) > 0) && sroot->left)
			sroot = sroot->left;
		else if(((compare = strcmp(sroot->suffix, s)) < 0) && sroot->right)
			sroot = sroot->right;
		else if((compare = strcmp(sroot->suffix, s)) == 0) {
			free(s);
			exists = 1;
			break;
		} else
			break;
	}

	if(exists) { // add name to sroot->vartree

		vp = calloc(1, sizeof(V_node));
		vp->name = name;

		vroot = sroot->vartree;
		while(1) {
			if(((compare = strcmp(vroot->name, name)) > 0) && vroot->left)
				vroot = vroot->left;
			else if(((compare = strcmp(vroot->name, name)) <= 0) && vroot->right)
				vroot = vroot->right;
			else
				break;
		}

		if(compare > 0)
			vroot->left = vp;
		else
			vroot->right = vp;

	} else { // create new suffix node and new vartree

		sp = calloc(1, sizeof(S_node));
		sp->vartree = calloc(1, sizeof(V_node));
		sp->suffix = s;
		sp->vartree->name = name;

		if(compare > 0)
			sroot->left = sp;
		else
			sroot->right = sp;
	}
}

int gettoken(void) {
	char *p;
	int c;

	while(isspace((c = getchar())));

	p = namebuf;

	if(isalpha(c)) {
		for(*p++ = c; isalnum((c = getchar())) || c == '_';)
			*p++ = c;
		*p = 0;

		ungetchar(c);

		return (tokentype = NAME);
	} else if(c == '/') { // skip past comments
		c = getchar();

		if(c == '/') {
			while(getchar() != '\n');
			return (tokentype = OTHER);
		} else if(c == '*') {
			while(1) {
				while(getchar() != '*');
				if(getchar() == '/')
					return (tokentype = OTHER);
			}
		}

		ungetchar(c);
	} else if(c == '[') {
		while(getchar() != ']');
		return (tokentype = ARR);
	} else if(c == '*') {
		return (tokentype = STAR);
	} else if(c == '{') {
		return (tokentype = RBRACE);
	} else if(c == '}') {
		return (tokentype = LBRACE);
	} else if(c == '=') {
		while((c = getchar()) != ';');

		ungetchar(c);

		return (tokentype = EQ);
	} else if(c == ';') {
		return (tokentype = SEMI);
	} else if(c == EOF) {
		return (tokentype = END);
	} else {
		return (tokentype = OTHER);
	}
}

// TODO parser finds suffixtree variable twice
int getvarname(void) {
	int n = 0;

	while(gettoken() != END) {
		if(tokentype == NAME) {
			if(strcmp(namebuf, "typedef") == 0
			|| strcmp(namebuf, "enum") == 0
			|| strcmp(namebuf, "include") == 0
			|| strcmp(namebuf, "continue") == 0
			|| strcmp(namebuf, "break") == 0
			|| strcmp(namebuf, "return") == 0
			|| strcmp(namebuf, "if") == 0
			|| strcmp(namebuf, "else") == 0
			|| strcmp(namebuf, "for") == 0
			|| strcmp(namebuf, "while") == 0
			|| strcmp(namebuf, "goto") == 0) {
				lastwasstruct ^= lastwasstruct;
				--n;
			} else if(strcmp(namebuf, "struct") == 0) {
				lastwasstruct = 1;
				--n;
			} else {
				++n;
			}
		} else if(tokentype == RBRACE && lastwasstruct) {
			while(gettoken() != LBRACE); // skip to end of struct decl
			n ^= n; // reset name count
		} else if(tokentype == OTHER) {
			n ^= n; // reset name count
		} else if(tokentype == STAR) {
			continue;
		} else if(tokentype == ARR) {
			continue;
		} else if(tokentype == EQ) {
			continue;
		} else if(tokentype == SEMI) {
			if(n >= 2)
				return 1;
			n ^= n;
		}
	}

	return 0;
}
