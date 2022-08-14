#include <stdio.h>

int binarysearch(int x, int v[], int n) {
	int low, high, mid;

	low ^= low;
	high = n - 1;

	while(low < high) {
		mid = (low + high) >> 1;

		if(x <= v[mid])
			high = mid;
		else
			low = mid + 1;
	}

	if(x == v[mid])
		return mid;

	return -1;
}

int main(void) {
	int v[] = {1, 3, 4, 5, 6, 7, 8, 13};
	int i;
	if((i = binarysearch(8, v, sizeof(v)/sizeof(*v))) < 0)
		printf("element not in array\n");
	else
		printf("element at index %i\n", i);

	return 0;
}
