#include "Bin.h"
#include <stdio.h>
#include <stdlib.h>

void print_dot_tree(FILE *fp, Bin<int> *root);
static void print_dot_tree_aux(FILE *fp, Bin<int> *node);

int main(int argc, char *argv[]) {
	int i, num;
	Bin<int> *root;
	FILE *fp;

	if (argc == 1) {
		fprintf(stderr, "usage: %s [N]\n", argv[0]);
		return 1;
	}
	num = atoi(argv[1]);
	root = new Bin<int>(0);
	for (i=1; i<num; ++i)
		root = bal_insert(root, i);
	
	fp = fopen("results.dot", "w");
	print_dot_tree(fp, root);
	fclose(fp);

	delete root;
	printf("Done. Run \"dot -Tpng results.dot -o graph.png\" to generate.\n");
	return 0;
}

void print_dot_tree(FILE *fp, Bin<int> *root) {
	fprintf(fp, "digraph BST {\n");
	fprintf(fp, "\tnode [fontname=\"Arial\"];\n");

	if (!root)
		fprintf(fp, "\n");
	else if (!root->l && !root->r)
		fprintf(fp, "\t%d;\n", root->t);
	else
		print_dot_tree_aux(fp, root);
	fprintf(fp, "}\n");
}

static void print_dot_tree_aux(FILE *fp, Bin<int> *node) {
	static int nnull = 0;

	if (node->l) {
		fprintf(fp, "\t%d -> %d;\n", node->t, node->l->t);
		print_dot_tree_aux(fp, node->l);
	} else {
		fprintf(fp, "\tnull%d [shape=point];\n", ++nnull);
		fprintf(fp, "\t%d -> null%d;\n", node->t, nnull);
	}

	if (node->r) {
		fprintf(fp, "\t%d -> %d;\n", node->t, node->r->t);
		print_dot_tree_aux(fp, node->r);
	} else {
		fprintf(fp, "\tnull%d [shape=point];\n", ++nnull);
		fprintf(fp, "\t%d -> null%d;\n", node->t, nnull);
	}
}
