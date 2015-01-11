CC = gcc
CFLAGS = -g -O3
DOT = /bin/dot

all:
	$(CC) $(CFLAGS) stack.c llist.c graph.c 2sat.c -o 2sat_main

graph: graph_visual.dot
	$(DOT) -Tpng graph_visual.dot -o graph_visual.png

graph_sccs: graph_sccs_visual.dot
	$(DOT) -Tpng graph_sccs_visual.dot -o graph_sccs_visual.png

clean:
	$(RM) 2sat_main graph_visual.* graph_sccs_visual.*
