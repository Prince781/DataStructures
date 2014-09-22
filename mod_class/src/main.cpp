#include "Mod.h"
#include <ctime>
#include <cassert>
#include <inttypes.h>
#include <cstdio>
 
const long max = INT64_MAX; // using this is ok for everything but division
const long min = INT64_MIN;
const long largest_prime = 9223372036854775783;
 
#define BENCHMARK(expr) BENCHMARK_(expr, #expr)
#define BENCHMARK_(expr, desc) \
        { \
                struct timespec start, end; \
                clock_gettime(CLOCK_MONOTONIC, &start); \
                { expr; } \
                clock_gettime(CLOCK_MONOTONIC, &end); \
                printf(desc " took %ld nsec\n", end.tv_nsec - start.tv_nsec); \
        }
 
void test_ctor() {
        Mod::set_modulus(17);
        Mod a(0), b(max), c = b, d = 4, e = 18, f(-3), g(f);
        assert(a.val() == 0);
        assert(b.val() == max % 17);
        assert(c.val() == max % 17);
        assert(c.val() == b.val());
        assert(d.val() == 4);
        assert(e.val() == 1);
        assert(f.val() == 14);
        assert(f.val() == g.val());
}
 
void test_cmp() {
        Mod::set_modulus(17);
        Mod a(-1), b(-1-17*1098), c(-1+17*91), d(16);
       
        Mod ms[] = {a,b,c,d};
       
        for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) {
                        assert(ms[i] == ms[j]); // operator==(Mod, Mod)
                        assert(16+17 == ms[i]); // operator==(long, Mod)
                }
                       
        Mod e(17*5);
        for (int i = 0; i < 4; ++i)
                assert(ms[i] != e);
}
 
void test_add() {
        Mod::set_modulus(7);
        Mod a(2), b(3), c(4), d = a+b, e = a+b+c+d;
        // make sure a and b haven't changed
        assert(a == 2);
        assert(b == 3);
        assert(d == 5);
        assert(e == Mod(2+3+4+2+3));
        a += 7;
        assert(a == 2);
        a += b;
        assert(a == 5);
       
        Mod::set_modulus(largest_prime);
        a = largest_prime-1;
        a += largest_prime-1;
        // (p-1+p-1) mod p = (2p-2) mod p = -2 mod p = p-2
        assert(a == largest_prime-2);
}
 
void test_sub() {
        Mod::set_modulus(20);
        Mod a(16), b(19), c(10);
        assert(a - b == -3);
        // make sure a and b haven't changed
        assert(a == 16);
        assert(b == 19);
       
        assert(a - c == 6);
        assert(c - b == -9);
        assert(c - b == 11);
       
        assert(Mod(min) == 12);
       
        assert(-a == 4);
       
        Mod::set_modulus(max);
        assert(min - Mod(2) == max-3);
        assert(-Mod(min) == 1);
}
 
void test_mul() {
        Mod::set_modulus(17);
        Mod a(3), b(4), c(5);
        assert(a*b == 12);
        assert(b*c == 3);
        assert(20*a == 9);
        assert(16*a == 14);
       
        Mod::set_modulus(6);
        assert(7*Mod(4) == 4);
       
        Mod::set_modulus(max);
        // (p-1)*(p-1) = (p-1)^2 = p^2 - 2p + 1
        // (p^2 - 2p + 1) mod p = 1 mod p = 1
        assert(Mod(max-1)*(max-1) == 1);
       
        BENCHMARK_(
        for (int i = 0; i < 1000000; ++i)
                Mod(i) * i;
        ,"1,000,000 i*i");
}
 
void test_inv() {
        Mod::set_modulus(largest_prime);
       
        BENCHMARK_(
        for (int i = 1; i < 100000; ++i)
                Mod(i).pwr(-1);
        ,"100,000 inv() calls without checks")
       
        BENCHMARK_(
        for (int i = 1; i < 100000; ++i) {
        	cout << "i = " << i << std::endl;
        	cout << "inverse = " << Mod(i).pwr(-1) << std::endl;
        	if (i * Mod(i).pwr(-1) != 1)
        		cout << "error: " << i << "*" << Mod(i).pwr(-1) << " = " 
        			<< i*Mod(i).pwr(-1) << std::endl;
                assert(i * Mod(i).pwr(-1) == 1);
        }
        ,"100,000 inv() calls with checks")
}
 
void test_div() {
        Mod::set_modulus(17);
        Mod a(2), b(3), c(8);
        assert(c / a == 4);
        assert(c / b == 14);
       
        Mod::set_modulus(largest_prime);
}
 
void test_pwr() {
        Mod::set_modulus(largest_prime);
       
        BENCHMARK_(
        for (int i = 0; i < 10000; ++i)
                Mod(3074115597533209342+i).pwr(4811390885768226372+i).pwr(1149483077098556915+i);
        ,"10,000 Mod().pwr().pwr() calls")
       
        BENCHMARK_(
                assert(Mod(5915587277).pwr(1500450271) == 794336068446396654)
        ,"1 pwr() assert")
}
 
 
int main() {
    BENCHMARK(test_ctor())
    BENCHMARK(test_cmp())
    BENCHMARK(test_add())
    BENCHMARK(test_sub())
    test_mul();
    test_inv();
    BENCHMARK(test_div())
    test_pwr();
   
    return 0;
}
