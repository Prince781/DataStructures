/**
 * partition assignment
 */
#include <stdio.h>
#include <stdlib.h>

void p(long n, long line);

long parts(long n);

int main(int argc, char *argv[]) {
	long n, line, ps;
	
	if (argc < 3) {
		printf("usage %s [n] [line]\n", argv[0]);
		return -1;
	}
	n = atol(argv[1]);
	line = atol(argv[2]);
	ps = parts(n);
	printf("p(%ld) = %ld\n", n, ps);
	p(n, line);
	return 0;
}

long partitions(long **part, long n, long k) {
	if (k == 0)
		return 0;
	if (n < 0)
		return 0;
	if (n == 0)
		return 1;

	if (part[n][k] == 0)
		part[n][k] = partitions(part, n, k-1) 
			   + partitions(part, n-k, k);
	return part[n][k];
}

long parts(long n) {
	long **arr = malloc((n+1) * sizeof(*arr));
	long i;

	for (i=0; i<=n; ++i)
		arr[i] = calloc(n+1, sizeof(**arr));

	long v = partitions(arr, n, n);
	
	for (i=0; i<=n; ++i)
		free(arr[i]);
	free(arr);
	return v;
}

void p(long n, long line) {
	long *arr = calloc(n, sizeof(*arr));
	long i, j, k;
	long r;	/* remaining */
	long l;

	arr[0] = n;
	for (l=0; arr[0] > 1 && l<line; ++l) {
		for (i=0; i < n && arr[i] > 1; ++i)
			;
		--i;
		r = n+1;
		for (j=k=0; j < n; ++j) {
			if (arr[j] > 0 && line-1 == l)
				printf("%d ", arr[j]);
			if (k <= i && arr[j] + 1 < arr[i])
				k = j;
			if (k-i == 1)
				arr[j] = r >= arr[i]-1 ? arr[i]-1 : r;
			r -= arr[j];
		}
		if (line-1 == l)
			printf("\n");
		--arr[i];
		if (k-i > 1)
			++arr[k];
	}
	if (l+1 == line) {
		for (j=0; j<n; ++j)
			printf("%d ", arr[j]);
		printf("\n");
	}
	free(arr);
}
