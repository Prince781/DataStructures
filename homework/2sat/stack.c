#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

static void stack_grow(struct stack *s, int addition);

struct stack *stack_new(int maxsize)
{
	struct stack *s;

	if ((s = malloc(sizeof(struct stack))) == NULL) {
		fprintf(stderr, "%s: could not create stack\n", __func__);
		return NULL;
	}
	s->data = malloc(maxsize * sizeof(void *));
	s->maxsize = maxsize;
	s->top = -1;

	return s;
}

void stack_push(struct stack *s, void *elem)
{
	if (stack_full(s))
		stack_grow(s, s->maxsize);
	s->data[++s->top] = elem;
}

void *stack_pop(struct stack *s)
{
	if (stack_empty(s)) {
		fprintf(stderr, "%s: stack is empty\n", __func__);
		return NULL;
	} else
		return s->data[s->top--];
}

void *stack_peek(const struct stack *s)
{
	if (stack_empty(s)) {
		fprintf(stderr, "%s: stack is empty\n", __func__);
		return NULL;
	} else
		return s->data[s->top];
}

void *stack_get(const struct stack *s, int index)
{
	if (index < 0 || index > s->top) {
		fprintf(stderr, "%s: %d is out of range\n", __func__, index);
		return NULL;
	} else
		return s->data[index];
}

static void stack_grow(struct stack *s, int addition)
{
	int nsize = s->maxsize + addition;
	void *p;

	if ((p = realloc(s->data, nsize * sizeof(*s->data))) == NULL)
		fprintf(stderr, "%s: could not resize stack (%d => %d)\n",
			__func__, s->maxsize, nsize);
	else {
		s->data = p;
		s->maxsize = nsize;
	}
}

void stack_destroy(struct stack *s)
{
	free(s->data);
	free(s);
}
