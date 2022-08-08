// two isupper

int isupper(int c) { // to save space
	return c >= 'A' && c <= 'Z';
}

#define isupper(c) (c >= 'A' && c <= 'Z') // to save time
