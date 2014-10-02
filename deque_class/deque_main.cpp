#include "deque.h"

int main(int argc, char *argv[]) {
	Deque<int> d1(4), d2;

	cout << "d1 size: " << d1.size() << endl;
	cout << "d2 size: " << d2.size() << endl;

	for (int i=1; i<2; ++i)
		d2.push_front(i);
	for (int i=1; i<2; ++i)
		cout << "d2: " << d2.pop_front() << endl;

	return 0;
}
