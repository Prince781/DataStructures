#include "Mod.h"
#include <cstdlib>
#include <inttypes.h>

#define HALF_MAX (INT64_MAX>>1)

#define _SMALLEST(a)\
((a) > HALF_MAX ? ((a)-(Mod::modulus)) : (a))

#define _MOD(a,m)\
((a) < 0 || (a) >= (m) ? ((a%m) + m)%(m) : a) // correct mod operator

#define _MAX(a,b) ((a) > (b) ? (a) : (b))

static unsigned long mod_mult(unsigned long a, unsigned long b,
	unsigned long c);

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
	x = mod_mult(x, m.x, Mod::modulus);
	return *this;
}

Mod& Mod::operator/=(const Mod& m) {
	x = mod_mult(x, m.pwr(-1).x, Mod::modulus);
	return *this;
}

Mod Mod::operator-(void) const {
	return Mod(-x);
}

Mod Mod::pwr(long e) const {
	long b;
	Mod x_new(0);
	long p, p2;

	if (e < 0) {
		x_new = inv(x);
		e = -e;
	} else if (e == 0)
		return 1;
	  else
		x_new = e&1 ? x : 1;
	b = x;	// assumption that x == x mod C already

	for (p2=p=1; (p2 < (p<<=1)) && p <= e; p2=p) {
		b = mod_mult(b, b, Mod::modulus);
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
	return Mod(mod_mult(a.x, b.x, Mod::modulus));
}

Mod operator*(long t, const Mod& m) {
	return Mod(mod_mult(_MOD(t,Mod::modulus), m.x, Mod::modulus));
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
		p0 = _MOD(_MOD(p0,Mod::modulus)-mod_mult(p1,q,Mod::modulus),
			Mod::modulus);
		// swap
		temp = p1;
		p1 = p0;
		p0 = temp;
	} while (r != 0);

	if (a == 1)
		return p0;
	else {
	noinverse:
		cout << "no inverse found" << std::endl;
		exit(-1);
		return -1;
	}
}

// use "Russian Peasant" multiplication to compute (a*b) mod m without overflow
static unsigned long mod_mult(unsigned long a,
	unsigned long b, unsigned long c) {
	long r;

	for (r=0; a!=0; a>>=1) {
		if (a & 1) r = (r+b)%c;
		b = (b << 1)%c;
	}
	return r;
}
