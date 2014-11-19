/*
 * Sorts an array of integers by partitioning them.
 * 11-18-14
 */
#include <stdlib.h>

void sort(unsigned *a, size_t len)
{
	unsigned *a2;
	int freq[0x10001] = {0}, freq2[0x10001] = {0};
	size_t i;

	a2 = malloc(len*sizeof(unsigned));
	for (i=0; i<len; ++i)
		++freq[1+(a[i] & 0xffff)];
	for (i=1; i<0x10001; ++i)
		freq[i] += freq[i-1];
	for (i=0; i<len; ++i)
		a2[freq[a[i]&0xffff]++] = a[i];
	for (i=0; i<len; ++i)
		++freq2[1+(a2[i]>>16)];
	for (i=1; i<0x10001; ++i)
		freq2[i] += freq2[i-1];
	for (i=0; i<len; ++i)
		a[freq2[a2[i]>>16]++] = a2[i];
	free(a2);
}

