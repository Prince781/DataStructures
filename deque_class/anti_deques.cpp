/**
 * anti_deques.cpp
 *  Created On: Oct 5, 2014
 * 	Author: Princeton Ferro
 */
#include "deque.h"
#include <algorithm>
#include <iostream>
#include <fstream>

using std::ofstream;

template<typename T>
bool dequeable(Deque<T>& deque, T *set, T *s_end);

template<typename T>
void printlist(ofstream& fp, T *start, T *end);

int main(int argc, char *argv[]) {
	int set[] = { 1, 2, 3, 4, 5 };
	int *p;
	Deque<int> test_deque(5);
	ofstream file;

	file.open("output.txt");

	for (p = set; p < set + 5; ++p)
		test_deque.push_back(*p);
//	cout << "is_set: " << test_deque.is_set(set, set+5) << endl;

	do {
		Deque<int> d(test_deque);
		if (!dequeable(d, set, set+5))
			printlist(file, set, set+5);
	} while (std::next_permutation(set, set + 5));

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
