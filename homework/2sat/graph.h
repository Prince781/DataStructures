#ifndef _GRAPH_H
#define _GRAPH_H

#include "llist.h"
#include "stack.h"

typedef struct vertex {
	struct llist *nbrs;	/* neighbors */
	struct llist *parents;	/* for transpose */
	int v;		/* value */
	char seen;		/* seen already or not? */
} Vertex;

Vertex *vertex_new(int val);

/* skew: skew indices by a value */
void vertex_tostring(Vertex *v, int n, char str[n], int skew);

void vertex_destroy(Vertex *vert);

typedef struct scc {
	struct llist *verts;	/* vertices in this SCC (except for leader) */
	Vertex *leader;		/* vertex where v = group number; this is for
				   easy access */
	int group;	/* SCC number */
} SCC;

SCC *scc_new(int group);

void scc_destroy(SCC *comp);

typedef struct sccs {
	struct llist *groups;	/* contains SCCs */
} SCCS;

SCCS *sccs_new(void);

#define sccs_size(sccs) ((sccs)->groups->size)

void sccs_add_vertex(SCCS *comps, Vertex *v, int group);

SCC *sccs_get(SCCS *comps, int group);

/* note: does NOT free vertices */
void sccs_destroy(SCCS *comps);

typedef struct graph {
	Vertex **vtrue;		/* +v */
	Vertex **vfalse;	/* -v */
	int size;		/* max size of _each_ vertex list */
	int elems;		/* actual number of vertices */
	SCCS *sccs;		/* strongly connected components */
} Graph;

#define NO_VERTEX	0

Graph *graph_new(int size);

#define graph_get(g,i) ((i)<0 ? (g)->vfalse[-(i)-1] : (g)->vtrue[(i)-1])

/* v2 (optionally) == 0 if v1 !implies v2 */
void add_edge(Graph *g, int v1, int v2);

typedef void (*dfs_pass1)(Vertex *, void *[], int, const struct stack *);

/* dfs function (f): (vertex, args, has_shrunken, internal_stack)
 * s_temp is temporary stack, where all visited nodes are placed 
 * 	- Differs from internal stack, which only contains nodes that have
 * 	  yet to be visited.
 */
void dfs(Graph *g, int vi, dfs_pass1 func, void *args[], struct stack *s_temp);

typedef void (*dfs_pass2)(Vertex *, void *[]);

/* perform DFS on transpose graph */
void dfs_trans(Graph *g, int vi, dfs_pass2 func, void *args[]);

/* resets seen value for each v in g */
void graph_reset(Graph *g);

/* generates a GraphViz document for the graph,
 * but without a SCC analysis */
void graph_visualize(Graph *g, const char *fname);

/* generates a visualization of the graph, with
 * SCCS present */
void graph_visualize_sccs(Graph *g, const char *fname);

/* note: this destroys every vertex in the graph as well */
void graph_destroy(Graph *g);

#endif	/* _GRAPH_H */
