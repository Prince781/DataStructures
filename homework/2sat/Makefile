CC = gcc
CFLAGS = -g -O3
DOT = /bin/dot
GRAPHFILES = $(wildcard *.dot)
GRAPHS = $(GRAPHFILES:%.dot=%.png)

all:
	$(CC) $(CFLAGS) stack.c llist.c graph.c 2sat.c -o 2sat_main

$(GRAPHS): %.png : %.dot
	$(DOT) -Tpng $< -o $@

clean_graphs:
	$(RM) $(GRAPHS) $(GRAPHFILES)

clean: clean_graphs
	$(RM) 2sat_main
