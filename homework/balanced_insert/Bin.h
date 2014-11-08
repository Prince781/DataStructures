/**
 * Princeton Ferro
 * Data Structures
 * 	Modified: November 8th, 2014
 *	Objective: implement balanced insert function for binary tree
 */
#ifndef _BIN_H_
#define _BIN_H_

inline int max(int a, int b) {
	return a > b ? a : b;
}

template<class T>
struct Bin {
	explicit Bin(const T& t, Bin *l = 0, Bin *r = 0) : t(t), l(l), r(r), bal(0) {}
	~Bin() { delete l; delete r; }
	T t;
	Bin *l, *r;
	int bal;	/* bal in {-1, 0, 1} */
};

template<class T>
long size(const Bin<T> *root) {
	if (!root) return 0;
	return 1 + size(root->l) + size(root->r);
}

template<class T>
int height(const Bin<T> *root) {
	if (!root) return -1;
	return 1 + max(height(root->l), height(root->r));
}

template<class T, class V>
void prefix(const Bin<T> *root, V visit) {
	if (root) {
		visit(root);
		prefix(root->l, visit);
		prefix(root->r, visit);
	}
}

template<class T, class V>
void reverse_prefix(const Bin<T> *root, V visit) {
	if (root) {
		visit(root);
		reverse_prefix(root->r, visit);
		reverse_prefix(root->l, visit);
	}
}

template<class T, class V>
void infix(const Bin<T> *root, V visit) {
	if (root) {
		infix(root->l, visit);
		visit(root);
		infix(root->r, visit);
	}
}

template<class T, class V>
void postfix(const Bin<T> *root, V visit) {
	if (root) {
		postfix(root->l, visit);
		postfix(root->r, visit);
		visit(root);
	}
}

template<class T>
void insert(Bin<T> *root, const T& t) {
	if (t < root->t)
		if (!root->l)
			root->l = new Bin<T>(t);
		else
			insert(root->l, t);
	else if (!root->r)
		root->r = new Bin<T>(t);
	else
		insert(root->r, t);
}

template<class T>
Bin<T> *lr(Bin<T> *root) {
	if (root->r) {
		Bin<T> *c = root->r;
		root->r = root->r->l;
		c->l = root;
		return c;
	} else
		return root;
}

template<class T>
Bin<T> *rr(Bin<T> *root) {
	if (root->l) {
		Bin<T> *a = root->l;
		root->l = root->l->r;
		a->r = root;
		return a;
	} else
		return root;
}

template<class T>
Bin<T> *flip(Bin<T> *root, Bin<T>* (&retval)) {
	int first;

	first = retval == 0;
	if (root->l) {
		retval = (Bin<T>*) 1;
		flip(root->l, retval)->r = root;
	} else
		retval = root;
	root->l = root->r;
	root->r = 0;
	return first ? retval : root;
}

template<class T>
Bin<T> *flip(Bin<T> *root) {
	Bin<T> *retval = 0;
	return flip(root, retval);
}

template<class T>
Bin<T> *bal_insert(Bin<T> *root, const T& t) {
	if (t < root->t)
		if (!root->l)
			root->l = new Bin<T>(t);
		else
			root->l = bal_insert(root->l, t);
	else if (!root->r)
		root->r = new Bin<T>(t);
	else
		root->r = bal_insert(root->r, t);
	/* retracing */
	root->bal = height(root->r) - height(root->l);
	if (root->bal > -2 && root->bal < 2)
		return root;
	else if (root->bal == -2) {
		if (root->l->bal == 1) {	/* left right case */
			root->l = lr(root->l);
			root->l->bal = -1;
		}
		root = rr(root);	/* left left case */
	} else {
		if (root->r->bal == -1) {	/* right left case */
			root->r = rr(root->r);
			root->r->bal = 1;
		}
		root = lr(root);	/* right right case */
	}
	root->bal = height(root->r) - height(root->l);
	return root;
}

#endif	/* _BIN_H_ */
