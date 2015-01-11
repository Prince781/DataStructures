/**
 * 2-satisfiability problem
 * Princeton Ferro
 * 1-12-15
 */
#include <stdio.h>
#include "graph.h"

const void *NOARGS[0];

void get_sccs(Graph *g);

static void pass1_func(Vertex *v, void *args[], int has_shrunken,
		       const struct stack *internal_stack);

static void pass2_func(Vertex *v, void *args[]);

int main(int argc, char *argv[])
{
	FILE *fin;
	Graph *graph;
	int nvals, nclauses;
	int *atrue, *afalse;
	int i, j, k;

	fin = fopen("2sat.in", "r");
	fscanf(fin, "%d %d", &nvals, &nclauses);
	graph = graph_new(nvals);
	for (i=0; i<nclauses; ++i) {
		fscanf(fin, "%d %d", &j, &k);
		add_edge(graph, -k, j);
		add_edge(graph, -j, k);
	}

	graph_visualize(graph, "graph_visual.dot");
	get_sccs(graph);
	graph_visualize_sccs(graph, "graph_sccs_visual.dot");

	graph_destroy(graph);
	fclose(fin);
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
	args[0] = (void *) finished_stack;
	args[1] = (void *) temp_stack;
	args[2] = (void *) components;
	for (vi=1; vi <= g->size; ++vi) {
		dfs(g, vi, &pass1_func, args, temp_stack);
		dfs(g, -vi, &pass1_func, args, temp_stack);
	}
	while (!stack_empty(temp_stack))
		stack_push(finished_stack, stack_pop(temp_stack));
	graph_reset(g);
	while (!stack_empty(finished_stack)) {
		vert = stack_pop(finished_stack);
		args[3] = (void *) &vert->v;
		dfs_trans(g, vert->v, &pass2_func, args);
	}
	stack_destroy(finished_stack);
	stack_destroy(temp_stack);
	g->sccs = components;
}

static void pass1_func(Vertex *v, void *args[], int old_size, 
		       const struct stack *internal_stack)
{
	/* args[0] = finished_stack
	 * args[1] = temp_stack */
	struct stack *finished = args[0];
	struct stack *temp = args[1];

	if (old_size > stack_size(internal_stack))
		while (!stack_empty(temp) && !stack_empty(internal_stack) 
			&& stack_peek(temp) != stack_peek(internal_stack))
			stack_push(finished, stack_pop(temp));
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
