/* gendata.c
 * generate random 2sat data
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
	if (argc < 4) {
		printf("usage: %s [file] [nvalues] [nclauses]\n", argv[0]);
		return 1;
	}
	
	FILE *fin = fopen(argv[1], "w");
	int nv = atoi(argv[2]);
	int nc = atoi(argv[3]);
	int i, neg1, neg2;
	int x1, x2;
	int v = 0;

	fprintf(fin, "%d %d\n", nv, nc);
	srand(time(NULL));
	for (i=0; i<nc; ++i) {
		neg1 = rand() < RAND_MAX/2 ? -1 : 1;
		neg2 = rand() < RAND_MAX/2 ? -1 : 1;
		x1 = 1 + (v++ % nv);
		x2 = 1 + (int)(((float)rand() / RAND_MAX) * nv);
		fprintf(fin, "%d %d\n", neg1*x1, neg2*x2);
	}

	fclose(fin);
}
