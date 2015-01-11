/* test distributed sort */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>

unsigned *genarr(size_t len, unsigned max);
void sort(unsigned *a, size_t len);
void printa(int *a, size_t len);

int main(int argc, char *argv[])
{
	unsigned *arr, *arr2;
	size_t len = 1003409;
	int cmp(const void *a, const void *b);

	arr = genarr(len, 1000);
	arr2 = malloc(len * sizeof(unsigned));
	memcpy(arr2, arr, len*sizeof(unsigned));

	if (len < 0xfff) {
		printf("unsorted: \n\t");
		printa(arr, len);
	}
	sort(arr, len);
	if (len < 0xfff) {
		printf("sorted: \n\t");
		printa(arr, len);
	}
	printf("sort: done\n");
	qsort(arr2, len, sizeof(unsigned), cmp);
	printf("qsort: done\n");
	if (memcmp(arr,arr2,len*sizeof(unsigned)))
		printf("\tarray is not sorted\n");
	else
		printf("\tarray is sorted\n");
	free(arr);
	free(arr2);
	return 0;
}

int cmp(const void *a, const void *b)
{
	return *(const int *)a > *(const int *)b;
}

unsigned *genarr(size_t len, unsigned max)
{
	unsigned *a;
	unsigned now = time(NULL);

	a = malloc(len * sizeof(unsigned));
	while (len-- > 0)
		a[len] = rand_r(&now) % (max?max:UINT_MAX);
	return a;
}

void printa(int *a, size_t len)
{
	while (len-- > 0)
		printf(" %4d ", *a++);
	printf("\n");
}
