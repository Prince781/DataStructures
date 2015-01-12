/**
 * 2-satisfiability problem
 * Princeton Ferro
 * 1-12-15
 */
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
// #define DEBUG
#define DEBUG_GRAPHS

#define VERT_TRUE	'T'
#define VERT_FALSE	'F'

const void *NOARGS[0];

void get_sccs(Graph *g);

static void pass1_func(Vertex *v, void *args[], int has_shrunken,
		       const struct stack *internal_stack);

static void pass2_func(Vertex *v, void *args[]);

void assign_values(Graph *g);

int clauses_test(Vertex *(*clauses)[2], int nc);

int main(int argc, char *argv[])
{
	Graph *graph;
	int nvals, nclauses;
	Vertex *(*clauses)[2];
	int i, j, k;

	scanf("%d %d", &nvals, &nclauses);
	graph = graph_new(nvals);
	clauses = malloc(nclauses * sizeof(Vertex *[2]));
	for (i=0; i<nclauses; ++i) {
		scanf("%d %d", &j, &k);
		add_edge(graph, -k, j);
		add_edge(graph, -j, k);
		clauses[i][0] = graph_get(graph, j);
		clauses[i][1] = graph_get(graph, k);
	}

	get_sccs(graph);
	graph_condense(graph);

	if (!graph_satisfiable(graph))
		printf("Not satisfiable\n");
	else {
		Vertex *v;
		assign_values(graph);
		if (!clauses_test(clauses, nclauses))
			printf("Failed to find solution."
			       " Guess I need to improve this... :(\n");
		for (i=1; i<=graph->size; ++i) {
			v = graph_get(graph, i);
			if (v != NULL)
				printf("%c\n", v->value);
		}
	}

#ifdef DEBUG_GRAPHS
	graph_visualize(graph, "graph_visual.dot");
	graph_visualize_sccs(graph, "graph_sccs_visual.dot");
	graph_visualize_condensed(graph, "graph_condensed_visual.dot");
#endif

	free(clauses);
	graph_destroy(graph);
	return 0;
}

/* uses Kosaraju's algorithm
 * fills a set of SCCs
 * a SCCS is a struct holding a list of SCCs
 * each SCC (in the list) has a group number and a list of vertices
 * the group number is the "leader" vertex for the SCC in the graph
 * @note: sets g->sccs to new memory; this is auto-freed when graph_destroy()
 * is called
 */
void get_sccs(Graph *g)
{
	struct stack *finished_stack, *temp_stack;
	int maxsize = g->elems;
	int vi;
	Vertex *vert;
	void *args[4];
	SCCS *components;

	/* idea: transfer temp_stack items to finished_stack
	 * when internal DFS stack decreases in size */
	finished_stack = stack_new(maxsize);
	temp_stack = stack_new(maxsize);
	components = sccs_new();
	vi = 1;	/* current vertex */
	args[0] = finished_stack;
	args[1] = temp_stack;
	args[2] = components;
	for (vi=1; vi <= g->size; ++vi) {
		dfs(g, vi, &pass1_func, args, temp_stack);
		while (!stack_empty(temp_stack)) {
			vert = stack_pop(temp_stack);
			if (!vert->finished) {
				stack_push(finished_stack, vert);
				vert->finished = 1;
			}
		}
		dfs(g, -vi, &pass1_func, args, temp_stack);
		while (!stack_empty(temp_stack)) {
			vert = stack_pop(temp_stack);
			if (!vert->finished) {
				stack_push(finished_stack, vert);
				vert->finished = 1;
			}
		}
	}
	graph_reset(g);
#ifdef DEBUG
	int i;
	char vbuf[30];
	printf("%s: finished stack:\n", __func__);
	printf("%s: ======= bottom =======\n", __func__);
	for (i=0; i<stack_size(finished_stack); ++i) {
		vert = stack_get(finished_stack, i);
		vertex_tostring(vert, 30, vbuf, 0);
		printf("%s: stack(%d) = %s\n", __func__, i, vbuf);
	}
	printf("%s: =======  top   =======\n", __func__);
#endif
	while (!stack_empty(finished_stack)) {
		vert = stack_pop(finished_stack);
		args[3] = &vert->v;
		dfs_trans(g, vert->v, &pass2_func, args);
	}
	stack_destroy(finished_stack);
	stack_destroy(temp_stack);
	graph_reset(g);
	g->sccs = components;
}

static void pass1_func(Vertex *v, void *args[], int old_size, 
		       const struct stack *internal_stack)
{
	/* args[0] = finished_stack
	 * args[1] = temp_stack */
	struct stack *finished = args[0];
	struct stack *temp = args[1];
	Vertex *vert;

	if (old_size > stack_size(internal_stack))
		while (!stack_empty(temp) && !stack_empty(internal_stack) 
			&& stack_peek(temp) != stack_peek(internal_stack)) {
			vert = stack_pop(temp);
			if (!vert->finished) {
				stack_push(finished, vert);
				vert->finished = 1;
			}
		}
}

static void pass2_func(Vertex *v, void *args[])
{
	/* args[0] = finished_stack
	 * args[1] = temp_stack
	 * args[2] = SCCS
	 * args[3] = scc group number
	 */
	SCCS *components = args[2];
	int group = *(int *) args[3];

	sccs_add_vertex(components, v, group);
}

/* after condensing graph, assign truth values
 * to each vertex */
void assign_values(Graph *g)
{
	struct llist_elem *el, *el2;
	SCC *comp;
	Vertex *v, *notv;

	llist_foreach(g->sccs->groups, el) {
		comp = el->data;
		v = comp->leader;
		if (v->value == VERT_TRUE || v->value == VERT_FALSE)
			continue;
		notv = graph_get(g, -v->v);
		v->value = VERT_FALSE;
		notv->value = VERT_TRUE;
		llist_foreach(comp->verts, el2) {
			v = el2->data;
			notv = graph_get(g, -v->v);
			v->value = VERT_FALSE;
			notv->value = VERT_TRUE;
		}
	}
}

/* test that our truth assignments are valid */
int clauses_test(Vertex *(*clauses)[2], int nc)
{
	char val = 1;
	Vertex *v1, *v2;
	int i;

	for (i=0; i<nc && val; ++i) {
		v1 = clauses[i][0];
		v2 = clauses[i][1];
		val &= (v1->value == VERT_TRUE || v2->value == VERT_TRUE);
	}

	return val;
}
