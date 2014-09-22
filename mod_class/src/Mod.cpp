#include "Mod.h"
#include <cstdlib>
#include <inttypes.h>

#define HALF_MAX (INT64_MAX>>1)

#define _SMALLEST(a)\
((a) > HALF_MAX ? ((a)-(Mod::modulus)) : (a))

#define _MOD(a,m)\
((a) < 0 || (a) >= (m) ? ((a%m) + m)%(m) : a) // correct mod operator

#define _MAX(a,b) ((a) > (b) ? (a) : (b))

long Mod::modulus = 17;

Mod::Mod(long t) : x(_MOD(t, Mod::modulus)) { }

Mod::Mod(const Mod& m) : x(_MOD(m.x, Mod::modulus)) { }

Mod& Mod::operator=(const Mod& m) {
	x = m.x;
	return *this;
}

Mod& Mod::operator+=(const Mod& m) {
	x = _MOD(_SMALLEST(x) + _SMALLEST(m.x),
		Mod::modulus);
	return *this;
}

Mod& Mod::operator-=(const Mod& m) {
	x = _MOD(_SMALLEST(x) - _SMALLEST(m.x), Mod::modulus);
	return *this;
}

Mod& Mod::operator*=(const Mod& m) {
	x = _MOD(_SMALLEST(x) * _SMALLEST(m.x), Mod::modulus);
	return *this;
}

Mod& Mod::operator/=(const Mod& m) {
	Mod x_new(m.pwr(-1));
	x = x_new.x;
	return *this;
}

Mod Mod::operator-(void) const {
	return Mod(-x);
}

Mod Mod::pwr(long e) const {
	long b;
	Mod x_new(0);
	long p;

	if (e < 0) {
		x_new = inv(x);
		e = -e;
	} else
		x_new = e&1 ? x : 1;
	b = x;	// assumption that x == x mod C already
	if (e == 1)
		return x_new;
	for (p=1; p < (p<<=1) && p <= e; ) {
		b = _MOD(b*b, Mod::modulus);
		if (e & p) x_new *= b;
	}

	return x_new;
}

long Mod::val(void) const {
	return x;
}

void Mod::set_modulus(long m) {
	if ((Mod::modulus = m) < 2)
		exit(-1);	// fail fast
}

long Mod::get_modulus(void) {
	return Mod::modulus;
}

Mod operator+(const Mod& a, const Mod& b) {
	return Mod(_SMALLEST(a.x) + _SMALLEST(b.x));
}

Mod operator+(long t, const Mod& m) {
	return Mod(_SMALLEST(_MOD(t,Mod::modulus)) + _SMALLEST(m.x));
}

Mod operator-(const Mod& a, const Mod& b) {
	return Mod(_SMALLEST(a.x) - _SMALLEST(b.x));
}

Mod operator-(long t, const Mod& m) {
	return Mod(_SMALLEST(_MOD(t,Mod::modulus)) - _SMALLEST(m.x));
}

Mod operator*(const Mod& a, const Mod& b) {
	return Mod(_SMALLEST(a.x) * _SMALLEST(b.x));
}

Mod operator*(long t, const Mod& m) {
	return Mod(_SMALLEST(_MOD(t,Mod::modulus)) * _SMALLEST(m.x));
}

Mod operator/(const Mod& a, const Mod& b) {
	return a * b.pwr(-1);
}

Mod operator/(long t, const Mod& m) {
	return t * m.pwr(-1);
}

bool operator==(const Mod& a, const Mod& b) {
	return a.x == b.x;
}

bool operator==(long t, const Mod& m) {
	return _MOD(t, Mod::modulus) == m.x;
}

bool operator!=(const Mod& a, const Mod& b) {
	return !(a == b); // our definition of != will always be consistent
}

bool operator!=(long t, const Mod& m) {
	return !(t == m); // consistency
}

istream& operator>>(istream& is, Mod& m) {
	long a;

	is >> a;
	m.x = _MOD(a, Mod::modulus);
	return is;
}

ostream& operator<<(ostream& os, const Mod& m) {
	os << m.x;
	return os;
}

Mod Mod::inv(long r0) { // use egcd method and return x in (r0)x + (mod)y = 1
	long q, r;
	long p0, p1, temp;
	long a, b;

	if (r0 == 0)
		goto noinverse;

	a = Mod::modulus;
	b = r0;
	p0 = 0;
	p1 = 1;

	do {
		q = a/b;
		r = a - b*q;
		a = b;
		b = r;
		p0 = _MOD(_MOD(p0,Mod::modulus) 
			  - _MOD(_SMALLEST(p1*q),Mod::modulus),
			Mod::modulus);
		// swap
		temp = p1;
		p1 = p0;
		p0 = temp;
	} while (r != 0);

	if (a == 1)
		return _MAX(p0, p1);
	else {
	noinverse:
		cout << "no inverse found" << std::endl;
		exit(-1);
		return -1;
	}
}
