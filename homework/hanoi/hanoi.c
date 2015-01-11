/**
 * Princeton Ferro
 * Data Structures
 * Oct. 26, 2014
 */
#include <stdio.h>
#include <stdlib.h>
#define TEST
#define BUFFSIZE	(~0u >> 1)
/*
 * Tower of Hanoi, with constraint that it is only possible to move between
 * adjacent towers (1->2, 2->3, 3->1).
 */
void hanoi(int n, int from, int to, int aux);

static int moves = 0;

int main(int argc, char *argv[])
{
	char *buf = malloc(BUFFSIZE);

	/* no point in outputting line-by-line, which is slow */
	setvbuf(stdout, buf, _IOFBF, BUFFSIZE);
#ifdef TEST
	int n;

	printf("Enter n: ");
	fflush(stdout);
	scanf("%d", &n);
	hanoi(n, 1, 2, 3);
	fprintf(stderr, "done in %d moves\n", moves);
#else
	if (argv[1])
		hanoi(atoi(argv[1]), 1, 2, 3);
	else
		hanoi(3, 1, 2, 3);
#endif
	fflush(stdout);
	return 0;
}

void hanoi(int n, int from, int to, int aux)
{
	if (n) {
		hanoi(n-1, from, to, aux);
		hanoi(n-1, to, aux, from);
		printf("%d: pile %d ==> pile %d\n", n, from, to);
		moves++;
		hanoi(n-1, aux, from, to);
		hanoi(n-1, from, to, aux);
	}
}
