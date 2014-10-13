#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>	/* srand, rand */
#include <time.h>
#include <math.h>	/* pow */

#define ITERS	100UL

/* bad flip with non-uniform distribution */
bool bad_flip(void);

/* return uniform distribution */
bool flip(void);

/* return true with probability m/n */
bool flop(int m, int n);

int gcd(int a, int b);

int main(int argc, char *argv[]) {
	double avg = 0;
	unsigned long n;
	int num = 300, denom = 1000;

	srand(time(NULL));
	for (n=0; n<=ITERS; avg /= ++n)
		avg = n*avg + flop(num, denom);
	
	printf("after %d iterations with true prob (%f), avg = %f\n",
		ITERS, (float)num / denom, avg);

	return 0;
}

bool bad_flip(void) {
	return (rand() % 100 > 98);
}

bool flip(void) {
	bool r1, r2;

	r1 = bad_flip();
	r2 = bad_flip();
	return (r1 ^ r2 ? r1 : flip());
}

bool flop(int m, int n) {
	bool val;
	int i, f;

	val = false;
	f = (int) ceil(log((float)(m > n/2 ? (n-m) : m)/n) / log(0.5));
	for (i=f; i>0; --i)
		val |= flip();
	return (m > n/2 ? val : !val);
}

int gcd(int a, int b) {
	if (a == 0) return b;
	return gcd(b%a, a);
}
