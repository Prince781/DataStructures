#ifndef _STACK_H
#define _STACK_H

#define STACK_MINSIZE	10

struct stack {
	void **data;
	int top;	/* position in stack */
	int maxsize;	/* maximum size of stack */
};

/* maxsize is initial; stack can grow if necessary */
struct stack *stack_new(int maxsize);

void stack_push(struct stack *s, void *elem);

void *stack_pop(struct stack *s);

void *stack_peek(const struct stack *s);

#define stack_empty(s) ((s)->top < 0)

#define stack_full(s) ((s)->top >= (s)->maxsize - 1)

#define stack_size(s) ((s)->top + 1)

void stack_destroy(struct stack *s);

#endif	/* _STACK_H */
