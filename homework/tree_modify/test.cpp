#include "Bin.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

/*
 * Generates a tree of height n+1 of the form:
 * 			 root
 * 			/    \
 * 		      l_1    null
 *		     /   \
 *		   l_2   r_1
 *		  /   \
 *		l_3   r_2
 *	       /   \
 *	     l_k  r_3
 *	     .  \
 *	    .   r_k
 *	   .
 *	  /
 *	 l_n
 *          \
 *	    r_n
 */
Bin<string> *generate(int n);

class PrintBin {
public:
	void operator()(const Bin<string> *root) {
		cout << str << root->t << endl;
		str += " ";
	}
private:
	string str;
};

int main(int argc, char *argv[]) {
	Bin<string> *tree;
	PrintBin visitor;

	tree = generate(32);
	prefix(tree, visitor);
	cout << endl << endl;
	tree = flip(tree);
	reverse_prefix(tree, visitor);

	delete tree;
	cout << "Done." << endl;
	return 0;
}

Bin<string> *generate(int n) {
	Bin<string> *root = 0, *bin = 0, *left = 0;
	int i;

	root = bin = new Bin<string>(string("root"));
	for (i=1; i<=n; ++i, bin=left) {
		stringstream s1, s2;
		s1 << "l_" << i;
		s2 << "r_" << i;
		left = new Bin<string>(s1.str());
		left->r = new Bin<string>(s2.str());
		bin->l = left;
	}
	return root;
}
