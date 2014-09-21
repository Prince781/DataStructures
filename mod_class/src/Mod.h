/*
    Assignment: Implement the following class. Make sure that it handles all
    signed 8-byte integers correctly and without overflow.
 
    For now, if the user attempts to divide by zero or set the modulus to
    something < 2, you can just "fail fast" with exit(-1).
 
    DUE: September 22, 2014 (by midnight!)
*/

#ifndef ____Mod__
#define ____Mod__

#include <iostream>
#include <cassert>

using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::cerr;

class Mod {
public:
	Mod(long t);
	Mod(const Mod& m);
	Mod& operator=(const Mod& m);
	Mod& operator+=(const Mod& m);
	Mod& operator-=(const Mod& m);
	Mod& operator*=(const Mod& m);
	Mod& operator/=(const Mod& m);
	Mod operator-(void) const;
	Mod pwr(long e) const;
	long val(void) const;

	static void set_modulus(long m);
	static long get_modulus(void);

	friend Mod operator+(const Mod& a, const Mod& b);
	friend Mod operator+(long t, const Mod& m);
	friend Mod operator-(const Mod& a, const Mod& b);
	friend Mod operator-(long t, const Mod& m);
	friend Mod operator*(const Mod& a, const Mod& b);
	friend Mod operator*(long t, const Mod& m);

	/* divide two mods */
	friend Mod operator/(const Mod& a, const Mod& b);
	friend Mod operator/(long t, const Mod& m);

	/* test congruence */
	friend bool operator==(const Mod& a, const Mod& b);
	friend bool operator==(long t, const Mod& m);

	/* test against congruence */
	friend bool operator!=(const Mod& a, const Mod& b);
	friend bool operator!=(long t, const Mod& m);

	/* print out representation */
	friend istream& operator>>(istream& is, Mod& m);
	friend ostream& operator<<(ostream& os, const Mod& m);

private:
	long x;
	static long modulus;

	static Mod inv(long r0);
};

#endif /* defined(____Mod__) */
