#ifndef _LLIST_H
#define _LLIST_H

struct llist_elem {
	struct llist_elem *next;
	void *data;
};

#define llelem_islast(lle) ((lle)->next == 0)

/* singly-linked list */
struct llist {
	struct llist_elem *base;
	int size;
};

struct llist *llist_new(void);

/* returns bool: if added */
int llist_add(struct llist *ll, void *elem);

/* returns bool: if removed */
int llist_remove(struct llist *ll, void *elem);

int llist_contains(struct llist *ll, void *elem);

void *llist_get(struct llist *ll, int index);

#define llist_empty(ll) ((ll)->size == 0)

#define llist_foreach(ll, e) for (e=ll->base; e!=0; e=e->next) 

void llist_destroy(struct llist *ll);

#endif	/* _LLIST_H */
