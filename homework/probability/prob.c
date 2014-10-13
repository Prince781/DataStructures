#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>	/* srand, rand */
#include <time.h>

#define ITERS	10000000UL

/* bad flip with non-uniform distribution */
bool bad_flip(void);

/* return uniform distribution */
bool flip(void);

/* return true with probability m/n */
bool flop(int m, int n);

int main(int argc, char *argv[]) {
	double avg = 0;
	unsigned long n;
	int num = 300, denom = 1000;

	srand(time(NULL));
	for (n=0; n<ITERS; avg /= ++n)
		avg = n*avg + flip();
	
	printf("after %d iterations of flip(), avg = %f\n", n, avg);

	avg = 0;
	for (n=0; n<=ITERS; avg /= ++n) {
		avg = n*avg + flop(num, denom);
		printf("iter %d / %d\n", n, ITERS);
	}
	
	printf("after %d iterations of flop() with true prob (%f), avg = %f\n",
		ITERS, (float)num / denom, avg);

	return 0;
}

bool bad_flip(void) {
	return (rand() % 100 > 98);
}

bool flip(void) {
	bool r1;
	while (!((r1 = bad_flip()) ^ bad_flip()));
	return r1;
}

bool flop(int m, int n) {
	unsigned val = 0;
	int p;

	for (p=0; p<8*sizeof(unsigned)-1; ++p, val <<= 1)
		val |= flip();
	
	return (val <= ((float) m / n) * ~0u);
}
