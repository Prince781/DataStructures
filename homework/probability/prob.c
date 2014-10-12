#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>	/* srand, rand */
#include <time.h>

#define ITERS	10000000UL

/* bad flip with non-uniform distribution */
bool bad_flip(void);

/* return uniform distribution */
bool flip(void);

int main(int argc, char *argv[]) {
	double avg = 0;
	unsigned long n;

	srand(time(NULL));
	for (n=0; n<=ITERS; avg /= ++n)
		avg = n*avg + flip();
	
	printf("after %d iterations, avg = %f\n", ITERS, avg);

	return 0;
}

bool bad_flip(void) {
	return (rand() % 100 > 1);
}

bool flip(void) {
	bool r1, r2;

	r1 = bad_flip();
	r2 = bad_flip();
	return (r1 ^ r2 ? r1 : !flip());
}
