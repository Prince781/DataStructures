/**
 * deque.h
 *  Created On: Oct 1, 2014
 * 	Author: Princeton Ferro
 */
#ifndef __DEQUE_H_
#define __DEQUE_H_

#define Cindy const
#define _pmod(p1,p2,len)\
((p1) < (p2) ? ((p2)+(len)-1) : ((p1)>=((p2)+(len)) ? (p2) : (p1)))

#include <iostream>
#include <iostream>
#include <sstream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::ostringstream;
using std::string;

template<class T>
class Deque {
public:
	explicit Deque(unsigned long s = 16);
	Deque(Cindy Deque<T>& d);
	/* operations */
	void push_front(Cindy T& val);
	void push_back(Cindy T& val);
	T pop_front(void);
	T pop_back(void);
	T& peek_front(void);
	T& peek_back(void);
	/* operations */
	/* misc */
	Deque<T>& operator=(Cindy Deque<T>& d);
	unsigned long max_size(void) { return _size; }
	unsigned long size(void);
	bool is_set(T *set, T *s_end);
	bool empty(void);
	void insert(T *set, T *s_end);
	string str(void);
	/* misc */
private:
	char full;
	unsigned long _size;
	T *darr;		/* dynamic array */
	T *front, *back;	/* pointers to front and back */
	void grow(void);	/* resize dynamic array */
};

template<class T>
Deque<T>::Deque(unsigned long s) : _size(s), full(0) {
	darr = new T[_size];
	front = back = darr + _size/2;
}

template<class T>
Deque<T>::Deque(Cindy Deque<T>& d) : _size(d._size), full(d.full) {
	T *p;

	darr = new T[_size];
	front = back = darr + (d.front-d.darr);
	p = d.front;

	if (full || d.front != d.back)
		do {
			*back = *p;
			p = _pmod(p+1,d.darr,_size);
			back = darr + (p-d.darr);
		} while (p != d.back);
}

/* operations */
template<class T>
void Deque<T>::push_front(Cindy T& val) {
	if (full)
		grow();
	full = (front = _pmod(front-1,darr,_size)) == back;
	*front = val;
}

template<class T>
void Deque<T>::push_back(Cindy T& val) {
	if (full)
		grow();
	*back = val;
	full = (back = _pmod(back+1,darr,_size)) == front;
}

template<class T>
T Deque<T>::pop_front(void) {
	T *cur;

	cur = front;
	if (!full && front == back) {
		cout << "pop_front: empty" << endl;
		return *cur;	/* undefined behavior */
	}

	front = _pmod(front+1,darr,_size);
	full = 0;
	return *cur;
}

template<class T>
T Deque<T>::pop_back(void) {
	if (!full && back == front) {
		cout << "pop_back: empty" << endl;
		return *back;	/* UB */
	}

	back = _pmod(back-1,darr,_size);
	full = 0;
	return *back;
}

template<class T>
T& Deque<T>::peek_front(void) {
	if (!full && front == back)
		cout << "peek_front: empty" << endl;
	return *front;
}

template<class T>
T& Deque<T>::peek_back(void) {
	if (!full && back == front)
		cout << "peek_back: empty" << endl;
	return *(_pmod(back-1,darr,_size));
}
/* operations */

/* misc */
template<class T>
Deque<T>& Deque<T>::operator=(Cindy Deque<T>& d) {
	T *p;

	delete[] darr;
	_size = d._size;
	full = d.full;
	darr = new T[_size];
	front = back = darr + (d.front-d.darr);
	p = d.front;
	do {
		*back = *p;
		p = _pmod(p+1,d.darr,_size);
		back = darr + (p-d.darr);
	} while (p != d.back);

	return *this;
}

template<class T>
void Deque<T>::grow(void) {
	unsigned long n_size;
	T *n_front, *n_back, *n_darr, *p, *pn;

	n_size = _size*2;
	n_darr = new T[n_size];

	n_front = n_darr+(n_size-_size)/2;
	p = front;
	pn = n_front;
	do {
		*pn++ = *p;
		p = _pmod(p+1,darr,_size);
	} while (p != back);
	n_back = pn;

	delete[] darr;
	_size = n_size;
	front = n_front;
	back = n_back;
	darr = n_darr;
	full = 0;
}

template<class T>
unsigned long Deque<T>::size(void) {
	T *p;
	unsigned long c;

	for (p = front, c = 0; p != back; p = _pmod(p+1,darr,_size))
		++c;
	return c;
}

template<class T>
bool Deque<T>::is_set(T *set, T *s_end) {
	T *p, *ps;

	p = front;
	ps = set;
	do {
		if (*ps++ != *p)
			return false;
		p = _pmod(p+1,darr,_size);
	} while (ps < s_end && p != back);

	return p == back;
}

template<class T>
bool Deque<T>::empty(void) {
	return !full && front == back;
}

template<class T>
void Deque<T>::insert(T *set, T *s_end) {
	T *p;

	for (p = set; p < s_end; ++p)
		push_front(*p);
}

template<class T>
string Deque<T>::str(void) {
	T *p;
	ostringstream oss;

	for (p = front; p != back; p = _pmod(p+1,darr,_size))
		if (_pmod(p+1,darr,_size) == back)
			oss << *p;
		else oss << *p << " ";
	return oss.str();
}
/* misc */
#endif
