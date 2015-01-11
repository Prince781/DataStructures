#include "llist.h"
#include <stdio.h>
#include <stdlib.h>

struct llist *llist_new(void)
{
	struct llist *ll;

	if ((ll = malloc(sizeof(struct llist))) == NULL) {
		fprintf(stderr, "%s: could not create list\n");
		return NULL;
	}
	ll->base = NULL;
	ll->size = 0;

	return ll;
}

int llist_add(struct llist *ll, void *elem)
{
	struct llist_elem **lleptr;

	for (lleptr = &ll->base; *lleptr != NULL; lleptr = &(*lleptr)->next)
		if ((*lleptr)->data == elem)
			return 0;
	
	if ((*lleptr = malloc(sizeof(struct llist_elem))) == NULL) {
		fprintf(stderr, "%s: could not create element\n", __func__);
		return 0;
	}
	(*lleptr)->data = elem;
	(*lleptr)->next = NULL;
	ll->size++;

	return 1;
}

int llist_remove(struct llist *ll, void *elem)
{
	struct llist_elem **lleptr;
	struct llist_elem *llelem;

	for (lleptr = &ll->base; *lleptr != NULL; lleptr = &(*lleptr)->next)
		if ((*lleptr)->data == elem)
			break;
	
	if (*lleptr == NULL)
		return 0;

	llelem = *lleptr;
	*lleptr = (*lleptr)->next;
	free(llelem);
	ll->size--;

	return 1;
}

int llist_contains(struct llist *ll, void *elem)
{
	struct llist_elem *llelem;

	for (llelem = ll->base; llelem != NULL; llelem = llelem->next)
		if (llelem->data == elem)
			return 1;
	
	return 0;
}

void *llist_get(struct llist *ll, int index)
{
	struct llist_elem *llelem;
	int p;

	llelem = ll->base;
	for (p=0; p<index && llelem != NULL; ++p)
		llelem = llelem->next;
	
	return llelem;
}

void llist_destroy(struct llist *ll)
{
	struct llist_elem *llelem;

	while (ll->size > 0)
		llist_remove(ll, ll->base->data);
	
	free(ll);
}
