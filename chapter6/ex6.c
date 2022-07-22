#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define HASHSIZE 128
#define MAXLINE 1024
#define MAXNAME 255
#define STRLEN(x) (sizeof(x) / sizeof(*x)) - 1 /* compile-time strlen */

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

static struct nlist *hashtab[HASHSIZE];
char inbuf[MAXLINE];
char outbuf[MAXLINE];
char namebuf[MAXNAME];

int state = 0;
enum { IF = 1, DONE };

int mygetline(char *buf, int max) {
	int c, n = 0;

	while((c = getchar()) != EOF && c != '\n' && n < (max - 1))
		buf[n++] = c;

	if(c == '\n')
		buf[n++] = c;

	buf[n] = 0;

	return n;
}

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

void tablefree(struct nlist *hashtab[HASHSIZE]) {
	struct nlist *np, *nextp;

	for(int i = 0; i < HASHSIZE; ++i) {
		if(!(np = hashtab[i]))
			continue;

		nextp = np->next;
		while(np) {
			free(np->name);
			free(np->defn);
			free(np);
			np = nextp;
			if(np)
				nextp = np->next;
		}
	}
}

int main(void) {
	struct nlist *np;
	char *pin, *pout, *id, *val;
	int len, count = 0;

	while((len = mygetline(inbuf, MAXLINE)) > 0) {
		++count;
		pin = inbuf;
		pout = outbuf;

		while(*pin) {
			if(*pin == '"') {

				for(*pout++ = *pin++; *pin && *pin != '"'; *pout++ = *pin++);

				continue;

			} else if(strstr(pin, "/*") == pin) {

				while(len > 0 && !strstr(pin, "*/")) {
					len = mygetline(inbuf, MAXLINE);
					pin = inbuf;
				}

				if(len <= 0) {
					fprintf(stderr, "error: open comment at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

				while(strstr(pin, "*/") != pin)
					++pin;
				pin += 2;

				continue;

			} else if(strstr(pin, "//") == pin) {

				break;

			} else if(strstr(pin, "#define") == pin) {

				pin += STRLEN("#define");

				while(isspace(*pin))
					++pin;

				if(isalpha(*pin) || *pin == '_') {
					id = pin;
					while(isalnum(*pin) || *pin == '_')
						++pin;
					*pin++ = 0;

					while(isspace(*pin)) ++pin;

					val = pin;
					while(isalnum(*pin) || *pin == '_' || *pin == '.')
						++pin;
					*pin = 0;

					define(id, val);

					break;
				}

			} else if(strstr(pin, "#undef") == pin) {

				pin += STRLEN("#undef");

				while(isspace(*pin))
					++pin;

				if(isalpha(*pin) || *pin == '_') {
					id = pin;
					while(isalnum(*pin) || *pin == '_')
						++pin;
					*pin++ = 0;

					undef(id);

					break;
				}

			} else if(strstr(pin, "#ifdef") == pin) {

				if(state) {
					fprintf(stderr, "error: nested #ifdef at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

				state = IF;

				pin += STRLEN("#ifdef");

				while(isspace(*pin))
					++pin;

				if(isalpha(*pin) || *pin == '_') {
					id = pin;
					while(isalnum(*pin) || *pin == '_')
						++pin;
					*pin++ = 0;

					if(lookup(id)) {
						state = DONE;
						break;
					}

					while(len > 0
						&& (strstr(inbuf, "#elif") != inbuf)
						&& (strstr(inbuf, "#else\n") != inbuf)
						&& (strstr(inbuf, "#endif\n") != inbuf))
						len = mygetline(inbuf, MAXLINE);

					if(len <= 0) {
						fprintf(stderr, "error: open #ifdef\n");
						tablefree(hashtab);
						exit(1);
					}

					pin = inbuf;
					pout = outbuf;

					continue;
				} else {
					fprintf(stderr, "error: bad #ifdef at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

			} else if(strstr(pin, "#ifndef") == pin) {

				if(state) {
					fprintf(stderr, "error: nested #ifndef at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

				state = IF;

				pin += STRLEN("#ifndef");

				while(isspace(*pin))
					++pin;

				if(isalpha(*pin) || *pin == '_') {
					id = pin;
					while(isalnum(*pin) || *pin == '_')
						++pin;
					*pin++ = 0;

					if(!lookup(id)) {
						state = DONE;
						break;
					}

					while(len > 0
						&& (strstr(inbuf, "#elif") != inbuf)
						&& (strstr(inbuf, "#else\n") != inbuf)
						&& (strstr(inbuf, "#endif\n") != inbuf))
						len = mygetline(inbuf, MAXLINE);

					if(len <= 0) {
						fprintf(stderr, "error: open #ifndef\n");
						tablefree(hashtab);
						exit(1);
					}

					pin = inbuf;
					pout = outbuf;

					continue;
				} else {
					fprintf(stderr, "error: bad #ifndef at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

			} else if(strstr(pin, "#elif") == pin) {

				if(state == DONE) {
					while(len > 0 && strstr(inbuf, "#endif\n") != inbuf)
						len = mygetline(inbuf, MAXLINE);

					if(len <= 0) {
						fprintf(stderr, "error: open #elif\n");
						tablefree(hashtab);
						exit(1);
					}

					pin = inbuf;
					continue;
				} else if(state == 0) {
					fprintf(stderr, "error: lone #elif at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

				pin += STRLEN("#elif");

				while(isspace(*pin))
					++pin;

				if(isalpha(*pin) || *pin == '_') {
					id = pin;
					while(isalnum(*pin) || *pin == '_')
						++pin;
					*pin++ = 0;

					if(lookup(id)) {
						state = DONE;
						break;
					}

					while(len > 0
						&& (strstr(inbuf, "#elif") != inbuf)
						&& (strstr(inbuf, "#else\n") != inbuf)
						&& (strstr(inbuf, "#endif\n") != inbuf))
						len = mygetline(inbuf, MAXLINE);

					if(len <= 0) {
						fprintf(stderr, "error: open #elif\n");
						tablefree(hashtab);
						exit(1);
					}

					pin = inbuf;
					pout = outbuf;

					continue;
				} else {
					fprintf(stderr, "error: bad #elif at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

			} else if(strstr(pin, "#else\n") == pin) {

				if(state == DONE) {
					while(len > 0 && strstr(inbuf, "#endif\n") != inbuf)
						len = mygetline(inbuf, MAXLINE);

					if(len <= 0) {
						fprintf(stderr, "error: open #else\n");
						tablefree(hashtab);
						exit(1);
					}

					pin = inbuf;
					continue;
				} else if(state == 0) {
					fprintf(stderr, "error: lone #else at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

				state = DONE;
				break;

			} else if(strstr(pin, "#endif") == pin) {

				if(!state) {
					fprintf(stderr, "error: lone #endif at line %i\n", count);
					fputs(inbuf, stderr);
					fputs("^^^^^^\n", stderr);
					tablefree(hashtab);
					exit(1);
				}

				state ^= state;
				break;

			} else if(isalpha(*pin)) { // read a name

				id = namebuf;

				while(isalnum(*pin) || *pin == '_')
					*id++ = *pin++;
				*id = 0;

				if((np = lookup(namebuf)))
					for(val = np->defn; *val; *pout++ = *val++);
				else
					for(val = namebuf; *val; *pout++ = *val++);

			} else { // do nothing
				*pout++ = *pin++;
			}
		}

		if(pout > outbuf) {
			*pout = 0;
			fputs(outbuf, stdout);
		}
	}

	tablefree(hashtab);

	return 0;
}
