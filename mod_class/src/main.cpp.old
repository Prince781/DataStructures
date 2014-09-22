// main.cpp
// test the Mod class extensively
// 9-20-14
// Princeton Ferro
#include "Mod.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace std;

#define _MOD(a,m) (((a%m) + m)%m) // correct mod operator
#define printm(a) a"(mod " << Mod::get_modulus() << ")"

#define printcongr(s1,r)\
cout << s1 << " \u2261 " << r << printm(" ") << std::endl

#define printincongr(s1,r)\
cout << s1 << " \u2247 " << r << printm(" ") << std::endl

#define printeq(s1,r)\
cout << s1 << " = " << r << std::endl

int old_main(int argc, char *argv[]) {
	cout << "sizeof(long) = " << sizeof(long) << std::endl;
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

int main2(int argc, char *argv[]) {
	if (argc < 2)
		exit(-1);
	FILE *file = fopen(argv[1], "r");

	long a, m;
	int i = 0;
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	while (fscanf(file, "%ld %ld", &a, &m) != EOF) {
		Mod::set_modulus(m);
		Mod n(a);
		if (n * n.pwr(-1) != 1) {
			printf("%ld (mod %ld) has inverse %ld\n", n.val(),
				m, n.pwr(-1).val());
			return 1;
		}
		++i;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	long diff = end.tv_nsec - start.tv_nsec;
	printf("%d inverses in %ld ns, or %.8lf s\n",
		i, diff, diff / 1000000000.0);
	fclose(file);
	return 0;
}

int main(int argc, char *argv[]) {
	Mod::set_modulus(9182237390125665823L);
	
	long iter;
	int i;
	struct timespec start, end;

	iter = argc > 1 ? atol(argv[1]) : 100000;
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (i=0; i<iter; ++i) {
		Mod a(7182237390125665823L), b(8182237390125665823L);
		a.pwr(b.val());
		if (i == 1)
			printeq(a << "^" << b.val() << " mod "
				<< Mod::get_modulus(), a.pwr(b.val()));
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	long diff = end.tv_nsec - start.tv_nsec;
	printf("%d inverses in %ld ns, or %.8lf s\n",
		i, diff, diff / 1000000000.0);
	return 0;
}
