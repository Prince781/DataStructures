#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define INDEX_SKEW (0)
// #define DEBUG


/* initializes all labels */
static void graphviz_print_labels(Graph *g, FILE *fin,
				  const char *pre, int skew);

/* prints v -> w; for every edge */
static void graphviz_print_edges(Graph *g, FILE *fin,
				 const char *pre, int skew);

Vertex *vertex_new(int val)
{
	Vertex *vert;

	if ((vert = malloc(sizeof(*vert))) == NULL) {
		fprintf(stderr, "%s: could not allocate new vertex\n", __func__);
		return NULL;
	}
	vert->nbrs = llist_new();
	vert->parents = llist_new();
	vert->v = val;
	vert->seen = 0;
	vert->finished = 0;
	vert->scc = NULL;

	return vert;
}

void vertex_tostring(Vertex *v, int n, char str[n], int skew)
{
	if (v->v < 0)
		snprintf(str, n, "~x%d", -v->v + skew);
	else
		snprintf(str, n, "x%d", v->v + skew);
}

void vertex_destroy(Vertex *vert)
{
	llist_destroy(vert->nbrs);
	llist_destroy(vert->parents);
	free(vert);
}

Graph *graph_new(int size)
{
	Graph *g;

	if ((g = malloc(sizeof(*g))) == NULL) {
		fprintf(stderr, "%s: could not allocate new graph\n", __func__);
		return NULL;
	}
	g->vtrue = calloc(size, sizeof(*g->vtrue));
	g->vfalse = calloc(size, sizeof(*g->vfalse));
	g->size = size;
	g->elems = 0;
	g->sccs = NULL;

	return g;
}

void add_edge(Graph *g, int v1, int v2)
{
	if (graph_get(g, v1) == NULL) {
		if (v1 < 0) g->vfalse[-v1-1] = vertex_new(v1);
		else g->vtrue[v1-1] = vertex_new(v1);
		++g->elems;
	}
	if (v2 != NO_VERTEX) {
		if (graph_get(g, v2) == NULL) {
			if (v2 < 0) g->vfalse[-v2-1] = vertex_new(v2);
			else g->vtrue[v2-1] = vertex_new(v2);
			++g->elems;
		}
		Vertex *vert1 = graph_get(g, v1);
		Vertex *vert2 = graph_get(g, v2);
		if (!llist_contains(vert1->nbrs, vert2))
			llist_add(vert1->nbrs, vert2);	/* add v2 to v1's neighbors */
		if (!llist_contains(vert2->parents, vert1))
			llist_add(vert2->parents, vert1); /* v2 is child of v1 */
#ifdef DEBUG
		int n = 40;
		char vbuf1[n], vbuf2[n];
		vertex_tostring(vert1, n, vbuf1, INDEX_SKEW);
		vertex_tostring(vert2, n, vbuf2, INDEX_SKEW);
		printf("%s: adding %s -> %s\n", __func__, vbuf1, vbuf2);
#endif
	}
}

void dfs(Graph *g, int vi, dfs_pass1 func, void *args[], struct stack *s_temp)
{
	struct stack *s;
	struct llist_elem *el;
	Vertex *v, *w;
	int old_size;

	if (vi < -g->size || vi > g->size)
		return;
	else if ((v = graph_get(g, vi)) == NULL)
		return;
	else if (v->seen)
		return;
	s = stack_new(g->size * 2);
	stack_push(s, v);

	while (!stack_empty(s)) {
		old_size = stack_size(s);
		v = stack_pop(s);
		if (!v->seen) {
			v->seen = 1;
			if (s_temp != NULL)
				stack_push(s_temp, v);
			llist_foreach(v->nbrs, el) {
				w = el->data;
				if (w == v) continue;
				stack_push(s, w);
				if (s_temp != NULL)
					stack_push(s_temp, w);
			}
		}
		if (func != NULL)
			(*func)(v, args, old_size, s);
	}
	stack_destroy(s);
}

void dfs_trans(Graph *g, int vi, dfs_pass2 func, void *args[])
{
	struct stack *s;
	struct llist_elem *el;
	Vertex *v, *w;

	if (vi < -g->size || vi > g->size)
		return;
	else if ((v = graph_get(g, vi)) == NULL)
		return;
	else if (v->seen)
		return;
	s = stack_new(g->size * 2);
	stack_push(s, v);

	while (!stack_empty(s)) {
		v = stack_pop(s);
		if (!v->seen) {
			v->seen = 1;
			if (func != NULL)
				(*func)(v, args);
			llist_foreach(v->parents, el) {
				w = el->data;
				if (w == v) continue;
				stack_push(s, w);
			}
		}
	}
	stack_destroy(s);
}

void graph_reset(Graph *g)
{
	int i;

	for (i=0; i<g->size; ++i) {
		if (g->vtrue[i] != NULL)
			g->vtrue[i]->seen = g->vtrue[i]->finished = 0;
		if (g->vfalse[i] != NULL)
			g->vfalse[i]->seen = g->vfalse[i]->finished = 0;
	}
}

static void graphviz_print_labels(Graph *g, FILE *fin, 
				  const char *pre, int skew)
{
	Vertex *v;
	int vi;
	const int n = 40;
	char buf1[n];
	const char *filledstyle = ", style=filled, color=gray";
	for (vi = 0; vi < g->size; ++vi) {
		if ((v = g->vtrue[vi]) != NULL) {
			vertex_tostring(v, n, buf1, skew);
			fprintf(fin, "%s\"%s\" [label=<x<sub>%d</sub>%c>%s];\n",
				pre, buf1, v->v + skew,
				v->value != 0 ? v->value : ' ',
				v->seen ? filledstyle : "");
		}
		if ((v = g->vfalse[vi]) != NULL) {
			vertex_tostring(v, n, buf1, skew);
			fprintf(fin, "%s\"%s\" [label=<~x<sub>%d</sub>%c>%s];\n",
				pre, buf1, -v->v + skew,
				v->value != 0 ? v->value : ' ',
				v->seen ? filledstyle : "");
		}
	}
}

static void graphviz_print_edges(Graph *g, FILE *fin,
				 const char *pre, int skew)
{
	struct llist_elem *el;
	Vertex *v, *vnbr;
	int vi;
	const int n = 40;
	char buf1[n];
	char buf2[n];

	for (vi = 0; vi < g->size; ++vi) {
		if ((v = g->vtrue[vi]) != NULL)
			llist_foreach(v->nbrs, el) {
				vnbr = el->data;
				vertex_tostring(v, n, buf1, skew);
				vertex_tostring(vnbr, n, buf2, skew);
				fprintf(fin, "%s\"%s\" -> \"%s\";\n",
					pre, buf1, buf2);
			}
		if ((v = g->vfalse[vi]) != NULL)
			llist_foreach(v->nbrs, el) {
				vnbr = el->data;
				vertex_tostring(v, n, buf1, skew);
				vertex_tostring(vnbr, n, buf2, skew);
				fprintf(fin, "%s\"%s\" -> \"%s\";\n", 
					pre, buf1, buf2);
			}
	}
}

void graph_visualize(Graph *g, const char *fname)
{
	FILE *fin = fopen(fname, "w");
	
	fprintf(fin, "digraph G {\n");
	graphviz_print_labels(g, fin, "\t", INDEX_SKEW);
	graphviz_print_edges(g, fin, "\t", INDEX_SKEW);
	fprintf(fin, "}\n");
	
	fclose(fin);
}

void graph_visualize_sccs(Graph *g, const char *fname)
{
	FILE *fin = fopen(fname, "w");
	struct llist_elem *el, *el2, *el3;
	Vertex *v, *vnbr;
	const int n = 40;
	char vbuf[n], vbuf2[n];
	SCC *comp;
	const int skew = INDEX_SKEW;
	struct stack *edges;	/* remaining edges outside of any SCCs */

	if (g->sccs == NULL) {
		fprintf(stderr, "%s: g->sccs is NULL\n", __func__);
		fclose(fin);
		return;
	}
	edges = stack_new(g->elems);
	fprintf(fin, "digraph SCCS {\n");
	graphviz_print_labels(g, fin, "\t", skew);
	llist_foreach(g->sccs->groups, el) {
		comp = el->data;
		fprintf(fin, "\tsubgraph \"cluster_%d\" {\n", 
			comp->group + skew);
		fprintf(fin, "\t\tstyle = filled;\n");
		llist_foreach(comp->leader->nbrs, el2) {
			vnbr = el2->data;
			if (vertex_group(vnbr) != comp->group) {
				stack_push(edges, comp->leader);
				stack_push(edges, vnbr);
				continue;
			}
			vertex_tostring(comp->leader, n, vbuf, skew);
			vertex_tostring(vnbr, n, vbuf2, skew);
			fprintf(fin, "\t\t\"%s\" -> \"%s\";\n", vbuf, vbuf2);
		}
		llist_foreach(comp->verts, el2) {
			v = el2->data;
			llist_foreach(v->nbrs, el3) {
				vnbr = el3->data;
				if (vertex_group(vnbr) != comp->group) {
					stack_push(edges, v);
					stack_push(edges, vnbr);
					continue;
				}
				vertex_tostring(v, n, vbuf, skew);
				vertex_tostring(vnbr, n, vbuf2, skew);
				fprintf(fin, "\t\t\"%s\" -> \"%s\";\n",
					vbuf, vbuf2);
			}
		}
		if (llist_empty(comp->verts)) {
			vertex_tostring(comp->leader, n, vbuf, skew);
			fprintf(fin, "\t\t\"%s\";\n", vbuf);
		}
		fprintf(fin, "\t\tlabel = \"SCC %d\";\n", comp->group + skew);
		fprintf(fin, "\t}\n");
	}
	while (!stack_empty(edges)) {
		vnbr = stack_pop(edges);
		v = stack_pop(edges);
		vertex_tostring(v, n, vbuf, skew);
		vertex_tostring(vnbr, n, vbuf2, skew);
		fprintf(fin, "\t\"%s\" -> \"%s\";\n", vbuf, vbuf2);
	}
	fprintf(fin, "}\n");
	fclose(fin);
	stack_destroy(edges);
}

void graph_condense(Graph *g)
{
	struct llist_elem *el, *el2, *el3;
	SCC *comp;
	Vertex *v, *vnbr;

	if (g->sccs == NULL) {
		fprintf(stderr, "%s: g->sccs == NULL\n", __func__);
		return;
	}
	llist_foreach(g->sccs->groups, el) {
		comp = el->data;
		v = comp->leader;
		llist_foreach(v->nbrs, el2) {
			vnbr = el2->data;
			printf("marco\n");
			printf("el2 == %p\n", el2);
			printf("el2->next == %p\n", el2->next);
			printf("v: %p, vnbr: %p\n", v, vnbr);
			fflush(stdout);
			if (vertex_group(v) != vertex_group(vnbr)) {
				printf("v id: %d\n", v->v);
				printf("vnbr id: %d\n", vnbr->v);
				llist_add(comp->sccs, vnbr->scc);
			}
			printf("polo\n");
		}
		llist_foreach(comp->verts, el2) {
			v = el2->data;
			llist_foreach(v->nbrs, el3) {
				vnbr = el3->data;
				if (vertex_group(v) != vertex_group(vnbr))
					llist_add(comp->sccs, vnbr->scc);
			}
		}
	}
}

void graph_visualize_condensed(Graph *g, const char *fname)
{
	FILE *fin = fopen(fname, "w");
	struct llist_elem *el, *el2;
	SCC *comp, *comp_nbr;
	const int n = 40;
	char vbuf[n];
	Vertex *v;

	if (g->sccs == NULL) {
		fprintf(stderr, "%s: g->sccs == NULL\n", __func__);
		fclose(fin);
		return;
	}
	fprintf(fin, "digraph G {\n");
	llist_foreach(g->sccs->groups, el) {
		comp = el->data;
		fprintf(fin, "\t\"SCC %d\" [label=<SCC %d:<br/> ",
			comp->group, comp->group);
		v = comp->leader;
		if (v->v < 0)
			snprintf(vbuf, n, "~x<sub>%d</sub>%c ", -v->v,
				 v->value != 0 ? v->value : ' ');
		else
			snprintf(vbuf, n, "x<sub>%d</sub>%c ", v->v,
				 v->value != 0 ? v->value : ' ');
		fprintf(fin, "%s", vbuf);
		llist_foreach(comp->verts, el2) {
			v = el2->data;
			if (v->v < 0)
				snprintf(vbuf, n, "~x<sub>%d</sub>%c ", -v->v,
					 v->value != 0 ? v->value : ' ');
			else
				snprintf(vbuf, n, "x<sub>%d</sub>%c ", v->v,
					 v->value != 0 ? v->value : ' ');
			fprintf(fin, "%s", vbuf);
		}
		fprintf(fin, ">];\n");
	}
	llist_foreach(g->sccs->groups, el) {
		comp = el->data;
		llist_foreach(comp->sccs, el2) {
			comp_nbr = el2->data;
			fprintf(fin, "\t\"SCC %d\" -> \"SCC %d\";\n",
				comp->group, comp_nbr->group);
		}
	}
	fprintf(fin, "}\n");
	fclose(fin);
}

int graph_satisfiable(Graph *g)
{
	struct llist_elem *el, *el2;
	Vertex *v, *notv;
	SCC *comp;

	llist_foreach(g->sccs->groups, el) {
		comp = el->data;
		/* test leader first */
		v = comp->leader;
		notv = graph_get(g, -v->v);
		if (vertex_group(v) == vertex_group(notv))
			return 0;
		llist_foreach(comp->verts, el2) {
			v = el2->data;
			notv = graph_get(g, -v->v);
			if (vertex_group(v) == vertex_group(notv))
				return 0;
		}
	}
	return 1;
}

void graph_destroy(Graph *g)
{
	int i;

	for (i=0; i<g->size; ++i) {
		if (g->vtrue[i] != NULL)
			vertex_destroy(g->vtrue[i]);
		if (g->vfalse[i] != NULL)
			vertex_destroy(g->vfalse[i]);
	}
	if (g->sccs != NULL)
		sccs_destroy(g->sccs);
	g->sccs = NULL;
	free(g->vtrue);
	free(g->vfalse);
	free(g);
}

SCC *scc_new(int group)
{
	SCC *comp;

	if ((comp = malloc(sizeof(*comp))) == NULL) {
		fprintf(stderr, "%s: could not allocate new SCC\n", __func__);
		return NULL;
	}
	comp->verts = llist_new();
	comp->group = group;
	comp->sccs = llist_new();
	comp->leader = NULL;
	return comp;
}

void scc_destroy(SCC *comp)
{
	llist_destroy(comp->sccs);
	llist_destroy(comp->verts);
	free(comp);
}

SCCS *sccs_new(void)
{
	SCCS *comps;

	if ((comps = malloc(sizeof(*comps))) == NULL) {
		fprintf(stderr, "%s: could not allocate new SCCS\n", __func__);
		return NULL;
	}
	comps->groups = llist_new();

	return comps;
}

SCC *sccs_get(SCCS *comps, int group)
{
	struct llist_elem *el;
	SCC *comp;

	llist_foreach(comps->groups, el) {
		comp = el->data;
		if (comp->group == group)
			return comp;
	}
	return NULL;
}

void sccs_destroy(SCCS *comps)
{
	struct llist_elem *el;
	SCC *comp;

	llist_foreach(comps->groups, el) {
		comp = el->data;
		scc_destroy(comp);
	}
	llist_destroy(comps->groups);
	free(comps);
}
