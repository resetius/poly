#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <math.h>
#include <string.h>

#include "poly.hpp"

extern "C" {
#include <cmocka.h>
}

using namespace std;
using namespace poly;

void test_term(void**) {
    Term a{2, {0, 2}, {1,2}};
    cout << to_string(a) << "\n";
    assert_true(to_string(a) == "2a^1c^2");

    Term b{31337, {1, 2, 3}, {2,3,1}};
    cout << to_string(b) << "\n";
    assert_true(to_string(b) == "31337b^2c^3d^1");

    Term c = a*b;
    cout << to_string(c) << "\n";
    assert_true(to_string(c) == "62674a^1b^2c^5d^1");
}

void test_poly_plus_term(void**) {
    Poly p = Poly{} + Term{2, {0, 2}, {1,2}};
    cout << to_string(p) << "\n";
    assert_true(to_string(p) == "2a^1c^2");
    p = p + Term{2, {0, 2}, {1,2}};
    assert_true(to_string(p) == "4a^1c^2");
    p = p + Term{31337, {1, 2, 3}, {2,3,1}};
    cout << to_string(p) << "\n";
    assert_true(to_string(p) == "4a^1c^2+31337b^2c^3d^1");
    p = p + Term{2, {0, 2}, {1,2}};
    assert_true(to_string(p) == "6a^1c^2+31337b^2c^3d^1");
}

void test_poly_plus_poly(void**) {
    Poly p1 = Poly{} + Term{1, {0}, {1}} + Term{1, {1}, {1}} + Term{1, {2}, {1}};
    cout << to_string(p1) << "\n";
    Poly p2 = Poly{} + Term{1, {2}, {1}} + Term{1, {3}, {1}} + Term{1, {4}, {1}};
    cout << to_string(p2) << "\n";
    Poly p3 = p1+p2;
    cout << to_string(p3) << "\n";
    assert_true(to_string(p3) == "1a^1+1b^1+2c^1+1d^1+1e^1");
    Poly _const = Poly{} + Term{2};
    p3 = p1+p2+_const;
    cout << to_string(p3) << "\n";
    assert_true(to_string(p3) == "2+1a^1+1b^1+2c^1+1d^1+1e^1");
}

void test_poly_mul_poly(void**) {
    Poly p1 = Poly{} + Term{1, {0}, {1}} + Term{1, {1}, {1}};
    Poly p2 = Poly{} + Term{1, {0}, {1}} + Term{-1, {1}, {1}};
    auto p = p1 * p1;
    cout << to_string(p) << "\n";
    assert_true(to_string(p) == "1a^2+2a^1b^1+1b^2");

    p = p1 * p2;
    cout << to_string(p) << "\n";
    assert_true(to_string(p) == "1a^2+-1b^2");

    Poly _const = Poly{} + Term{2};
    p = p1 * _const;
    cout << to_string(_const) << "\n";
    cout << to_string(p) << "\n";
    assert_true(to_string(p) == "2a^1+2b^1");
}

void test_poly_pow(void**) {
    Poly p1 = Poly{} + Term{1, {0}, {1}} + Term{1, {1}, {1}};
    Poly p2 = Poly{} + Term{1, {0}, {1}} + Term{-1, {1}, {1}};

    auto p = p1^2;
    cout << to_string(p) << "\n";
    assert_true(to_string(p) == "1a^2+2a^1b^1+1b^2");
    p = p1^3;
    cout << to_string(p) << "\n";
    assert_true(to_string(p) == "1a^3+3a^1b^2+3a^2b^1+1b^3");
    p = p1^7;
    cout << to_string(p) << "\n";
    assert_true(to_string(p) == "1a^7+7a^1b^6+21a^2b^5+35a^3b^4+35a^4b^3+21a^5b^2+7a^6b^1+1b^7");
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_term),
        cmocka_unit_test(test_poly_plus_term),
        cmocka_unit_test(test_poly_plus_poly),
        cmocka_unit_test(test_poly_mul_poly),
        cmocka_unit_test(test_poly_pow)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

