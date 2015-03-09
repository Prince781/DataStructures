/**
 * partition assignment
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part(char *, int);

main() { part("", 5); }

void part(char *pre, int n) {
	int i;
	char *saved = strdup(pre), *tmp = 0;
	for (i=1; i<=n/2; ++i) {
		printf("%s%d + %d\n", saved, n-i, i);
		if (i > 1) {
			asprintf(&tmp, "%d + ", n-i);
			part(tmp, i);
			free(tmp);
			tmp = 0;
		}
		part("", n-i);
	}
	free(saved);
}
