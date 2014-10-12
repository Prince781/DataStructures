/*
 * main.cpp
 * test floyd.cpp
 */
#include "floyd.h"

/*
 * Initializes a list with a specified tail and loop length
 * @param p pointer to base pointer (assumed to be NULL)
 * @param tail length of tail
 * @param loop length of loop
 */
void init_list(Link<int> **p, int tail, int loop);

/*
 * Prints out a linked list (looped or unlooped) with length.
 * @param b base pointer
 * @param length absolute length of list
 */
void printl(Link<int> *b, int length);

int main(int argc, char *argv[]) {
	Link<int> *base = NULL;	/* base pointer */
	int tlen, llen, tlen_v, llen_v;

	printf("enter tail length and loop length: ");
	scanf("%d %d", &tlen, &llen);
	init_list(&base, tlen, llen);
	printf("initialized list (tail=%d, loop=%d)\n", tlen, llen);

	printl(base, tlen+llen);

	floyd(base, tlen_v, llen_v);
	printf("found list had (tail=%d, loop=%d)\n", tlen_v, llen_v);

	return 0;
}

void init_list(Link<int> **p, int tail, int loop) {
	if (*p != NULL) {
		fprintf(stderr, "init_list: base pointer not NULL\n");
		return;
	}
	
	int n;
	Link<int> **tail_end = NULL;

	for (n=0; n<tail; ++n) {
		*p = new Link<int>(n);
		(*p)->info = n;
		(*p)->next = NULL;
		p = &(*p)->next;
	}

	tail_end = p;

	for (; n<tail+loop; ++n) {
		*p = new Link<int>(n);
		(*p)->info = n;
		(*p)->next = *tail_end;
		p = &(*p)->next;
	}
}

void printl(Link<int> *b, int length) {
	int n;

	printf("linked list (size=%d):\n", length);
	for (n=0; n<length && b != NULL; ++n, b = b->next)
		if (b->next)
			printf("\tp_%u->next = p_%d\n", b->info,
				b->next->info);
		else
			printf("\tp_%u->next = NULL\n", b->info);
}
