CC = gcc
CFLAGS = -g -O3
DOT = /bin/dot
GRAPHS = $(wildcard *.dot)

all:
	$(CC) $(CFLAGS) stack.c llist.c graph.c 2sat.c -o 2sat_main

$(GRAPHS): %.dot : %.png
	$(DOT) -Tpng $< -o $@

clean:
	$(RM) 2sat_main $(GRAPHS)
