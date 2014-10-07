/**
 * anti_deques.cpp
 *  Created On: Oct 5, 2014
 * 	Author: Princeton Ferro
 */
#include "deque.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using std::ofstream;
using std::vector;
using std::set;

template<typename T>
bool dequeable(Deque<T>& deque, T *set, T *s_end);

template<typename T>
void printlist(ofstream& fp, T *start, T *end);

int main(int argc, char *argv[]) {
	ofstream file;
	int vals[] = { 1, 2, 3, 4, 5 };
	set<int> input(vals, vals+5);
	vector< set<int> > perms;

	file.open("output.txt");
//	cout << "is_set: " << test_deque.is_set(set, set+5) << endl;

	do {
		perms.push_back(input);
	} while (std::next_permutation(input.begin(), input.end()));

	for (vector< set<int> >::iterator it = perms.begin(); 
		it != perms.end(); ++it)
		cout << *it << endl;
	cout << "size = " << perms.size() << endl;

	file.close();
}

/**
 * Generate all combinations of deque operations, and see if any of these
 * yield an order that is our set.
 */
template<typename T>
bool dequeable(Deque<T>& deque, T *set, T *s_end) {
	if (set == s_end)
		return true;
	else if (deque.peek_front() == *set) {
		deque.pop_front();
		return dequeable(deque, set+1, s_end);
	} else if (deque.peek_back() == *set) {
		deque.pop_back();
		return dequeable(deque, set+1, s_end);
	} else
		return false;
}

/* print a list */
template<typename T>
void printlist(ofstream& fp, T *start, T *end) {
	T *p;

	for (p=start; p<end; ++p)
		fp << *p << " ";
	fp << endl;
}

/* print a set */

