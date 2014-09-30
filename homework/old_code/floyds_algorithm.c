/*
 * floyds_algorithm.c
 *  Created On: Sep 26, 2014
 * 	Author: Princeton Ferro
 *	Description: Using Floyd's Algorithm, take a graph (linked list) of
 *		     length N, with a loop of length L and tail of length T,
 *		     and find L and T.
 */
#include <stdio.h>
#include <stdlib.h>

struct list {
	struct list *next;	/* pointer to next item */
	unsigned	n;	/* number (for debug) */
};

/*
 * Initializes a list with a specified tail and loop length
 * @param p pointer to base pointer (assumed to be NULL)
 * @param tail length of tail
 * @param loop length of loop
 */
void init_list(struct list **p, unsigned tail, unsigned loop);

/*
 * Prints out a linked list (looped or unlooped) with length.
 * @param b base pointer
 * @param length absolute length of list
 */
void printl(struct list *b, unsigned length);

/*
 * Gets the tail and loop length of a linked list.
 * @param b base pointer
 * @param tail pointer to tail length variable (to fill)
 * @param loop pointer to loop length variable (to fill)
 */
void floyd(struct list *b, unsigned *tail, unsigned *loop);

int main(int argc, char *argv[]) {
	struct list *base = NULL;	/* base pointer */
	int tlen, llen, tlen_v, llen_v;

	printf("enter tail length and loop length: ");
	scanf("%d %d", &tlen, &llen);
	init_list(&base, tlen, llen);
	printf("initialized list (tail=%d, loop=%d)\n", tlen, llen);

//	printl(base, tlen+llen);

	floyd(base, &tlen_v, &llen_v);
	printf("found list had (tail=%d, loop=%d)\n", tlen_v, llen_v);

	return 0;
}

void init_list(struct list **p, unsigned tail, unsigned loop) {
	if (*p != NULL) {
		fprintf(stderr, "init_list: base pointer not NULL\n");
		return;
	}
	
	int n;
	struct list **tail_end = NULL;

	for (n=0; n<tail; ++n) {
		*p = malloc(sizeof(*p));
		(*p)->n = n;
		(*p)->next = NULL;
		p = &(*p)->next;
	}

	tail_end = p;

	for (; n<tail+loop; ++n) {
		*p = malloc(sizeof(*p));
		(*p)->n = n;
		(*p)->next = *tail_end;
		p = &(*p)->next;
	}
}

void printl(struct list *b, unsigned length) {
	int n;

	printf("linked list (size=%d):\n", length);
	for (n=0; n<length && b != NULL; ++n, b = b->next)
		if (b->next)
			printf("\tp_%u->next = p_%d\n", b->n, b->next->n);
		else
			printf("\tp_%u->next = NULL\n", b->n);
}

void floyd(struct list *b, unsigned *tail, unsigned *loop) {
	struct list *ptr1, *ptr2;
	unsigned p1, tlen, llen;

	*tail = *loop = 0;

	ptr1 = ptr2 = b;
	p1 = 0;
	do {
		if (ptr1) {
			ptr1 = ptr1->next;
			++p1;
		}
		if (ptr2)
			if (ptr2 = ptr2->next)
				ptr2 = ptr2->next;
	} while (ptr1 != ptr2);
	
	if (!ptr1)	/* reached end of tail (with no loop) */
		*tail = p1;
	else {
		for (llen=tlen=0; b != ptr1; ++tlen, ++p1) {
			b = b->next;
			ptr1 = ptr1->next;
			if (ptr1 == ptr2 && !llen)
				llen = tlen+1;
		}
		*tail = tlen;
		*loop = llen != 0 ? llen : p1 - tlen;
	}
}
