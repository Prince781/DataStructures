/**
 * anti_deques.cpp
 *  Created On: Oct 5, 2014
 * 	Author: Princeton Ferro
 */
#include "deque.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

using std::ofstream;
using std::string;
using std::vector;
using std::unordered_set;	/* constant time access; requires C++ 11 */
using std::next_permutation;

/* remove all dequeable elements from the set */
template<typename T>
void find_dequeable(unordered_set<string>& ds, Deque<T> in,
		    Deque<T> dq = Deque<T>(), Deque<T> out = Deque<T>());

int main(int argc, char *argv[]) {
	ofstream file;
	int elems[] = { 1, 2, 3, 4, 5 };	/* input elements */
	unordered_set<string> perms;		/* permutations of input */
	Deque<int> input;
	
	input.insert(elems, elems+5);

	file.open("output.txt");
	do {
		Deque<int> temp;
		temp.insert(elems, elems+5);
		perms.insert(temp.str());
	} while (next_permutation(elems, elems+5));

	find_dequeable(perms, input);

	for (unordered_set<string>::iterator it = perms.begin();
		it != perms.end(); ++it)
		file << *it << endl;

	file.close();
	return 0;
}

template<typename T>
void find_dequeable(unordered_set<string>& ds, Deque<T> in,
		    Deque<T> dq = Deque<T>(), Deque<T> out = Deque<T>()) {

	if (in.empty() && dq.empty()) {	/* out is full */
		string setstr = out.str();
		ds.erase(setstr);
		return;
	}

	if (!in.empty()) {
		/* push front, back to deque */
		Deque<T> in_new1(in), in_new2(in);
		Deque<T> dq_new1(dq), dq_new2(dq);
		Deque<T> out_new1(out), out_new2(out);

		dq_new1.push_front(in_new1.pop_front());
		dq_new2.push_back(in_new2.pop_front());

		cout << "input size: " << in.size() << endl;

		find_dequeable(ds, in_new1, dq_new1, out_new1);
		find_dequeable(ds, in_new2, dq_new2, out_new2);
	}

	if (!dq.empty()) {	/* pop front, back from deque */
		Deque<T> in_new3(in), in_new4(in);
		Deque<T> dq_new3(dq), dq_new4(dq);
		Deque<T> out_new3(out), out_new4(out);

		/* input does not change */
		out_new3.push_front(dq_new3.pop_front());
		out_new4.push_front(dq_new4.pop_back());

		find_dequeable(ds, in_new3, dq_new3, out_new3);
		find_dequeable(ds, in_new4, dq_new4, out_new4);
	}
}
