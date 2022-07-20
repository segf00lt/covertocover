#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];

unsigned int hash(char *s) {
	unsigned int hashval;

	for(hashval = 0; *s; ++s)
		hashval = *s + 31 * hashval;

	return hashval % HASHSIZE;
}

struct nlist *lookup(char *s) {
	struct nlist *np;

	for(np = hashtab[hash(s)]; np; np = np->next)
		if(strcmp(s, np->name) == 0)
			return np;

	return 0;
}

int define(char *name, char *defn) {
	struct nlist *np;
	unsigned int hashval;

	if((np = lookup(name)) == 0) {
		np = malloc(sizeof(struct nlist));
		
		if(np == 0 || (np->name = strdup(name)) == 0)
			return 0;

		hashval = hash(name);
		np->next = hashtab[hashval]; // new entries are added at the beginning of the chain
		hashtab[hashval] = np;
	} else
		free(np->defn);

	if((np->defn = strdup(defn)) == 0)
		return 0;

	return 1;
}

int undef(char *s) {
	struct nlist *np, *prevp;
	unsigned int hashval;

	np = prevp = 0;
	for(np = hashtab[(hashval = hash(s))]; np; prevp = np, np = np->next)
		if(!strcmp(s, np->name))
			break;

	if(!np)
		return 0;

	if(prevp)
		prevp->next = np->next;
	else
		hashtab[hashval] = np->next;

	free(np->name);
	free(np->defn);
	free(np);

	return 1;
}

int main(void) {
	define("MAX", "1024");
	define("MAXLINE", "256");
	undef("MAX");
	undef("MAXLINE");

	return 0;
}
