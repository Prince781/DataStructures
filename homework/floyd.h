/*
 * Floyd's Algorithm solution
 *  Created On: Sep 26, 2014
 *  Updated On: Sep 30, 2014
 * 	Author: Princeton Ferro
 *	Description: Using Floyd's Algorithm, take a graph (linked list) of
 *		     length N, with a loop of length L and tail of length T,
 *		     and find L and T.
 *	Update: Convert to C++
 */
#ifndef __FLOYD_H
#define __FLOYD_H

#include <cstdio>	/* NULL */

template<class T>
struct Link {
	T info;	/* information */
	Link *next;
	explicit Link(const T& t, Link *nxt = NULL) : info(t), next(nxt) {}
};

/*
 * Gets the tail and loop length of a linked list.
 * @param p base pointer
 * @param tail tail length variable (to fill)
 * @param loop loop length variable (to fill)
 */
template<class T>
void floyd(Link<T> *p, int& tail, int& loop) {
	Link<T> *ptr1, *ptr2;
	int p1, tlen, llen;

	tail = loop = 0;

	ptr1 = ptr2 = p;
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
		tail = p1;
	else {
		for (llen=tlen=0; p != ptr1; ++tlen, ++p1) {
			p = p->next;
			ptr1 = ptr1->next;
			if (ptr1 == ptr2 && !llen)
				llen = tlen+1;
		}
		tail = tlen;
		loop = llen != 0 ? llen : p1 - tlen;
	}
}


#endif
