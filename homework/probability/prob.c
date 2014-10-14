#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>	/* srand, rand */
#include "prob_benchmark.h"

#define ITERS	10000000UL

/* bad flip with non-uniform distribution */
bool bad_flip(void);

/* return uniform distribution */
bool flip(void);

/* return true with probability m/n */
bool flop(int m, int n);

int main(int argc, char *argv[]) {
	srand(time(NULL));

	benchmark_flip_test(flip);
	benchmark_flop_test_all_p(flop);
	benchmark_flop_test(flop,3,100);
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
