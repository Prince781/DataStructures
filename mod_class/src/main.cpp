// main.cpp
// test the Mod class extensively
// 9-20-14
// Princeton Ferro
#include "Mod.h"
#include <cstdlib>

#define printm(a) a"(mod " << Mod::get_modulus() << ")"

#define printcongr(s1,r)\
cout << s1 << " \u2261 " << r << printm(" ") << std::endl

#define printincongr(s1,r)\
cout << s1 << " \u2247 " << r << printm(" ") << std::endl

#define printeq(s1,r)\
cout << s1 << " = " << r << std::endl

int main(int argc, char *argv[]) {
	if (argc > 1) {
		Mod::set_modulus(atol(argv[1]));
		cout << "Set modulus to " << argv[1] << std::endl;
	}

	Mod a(1), b(1);
	long power;
	long g;

	cout << "enter two numbers" << std::endl;
	cin >> a;
	cin >> b;

	cout << " got " << a.val() << std::endl;
	cout << " got " << b.val() << std::endl;

	a == b ? printcongr(a,b) : printincongr(a,b);

	// add
	printeq(a << " + " << b, a+b);
	printeq(b << " + " << a, b+a);
	// subtract
	printeq(a << " - " << b, a-b);
	printeq(b << " - " << a, b-a);
	// multiply
	printeq(a << " * " << b, a*b);
	printeq(b << " * " << a, b*a);
	// division
	printeq(a << " / " << b, a/b);
	printeq(a << " / " << a, a / a);
	// negative multiplication
	printeq("-" << a << " * " << b, -a * b);
	printeq("-" << b << " * " << a, -b * a);
	// unary negation
	printcongr("-" << a, -a);
	printcongr("-" << b, -b);
	printeq("-" << a << " + " << "-" << b, -a + -b);
	// double negation
	printcongr("-" << -a, -(-a));
	printcongr("-" << -b, -(-b));
	// exponentiation
	cout << "enter a power to raise " << a << " by: ";
	cin >> power;
	printeq(a << "^" << power, a.pwr(power));

	return 0;
}
