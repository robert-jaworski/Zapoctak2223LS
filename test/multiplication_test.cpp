#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;
using namespace number_utils;

int run_test() {
    matrix<double> m1(3, 3, { 1, 2, 3, 1, 2, 3, 1, 2, 3 }), m2(3, 3, { 1, 1, 1, 2, 2, 2, 3, 3, 3 });

    cout << "m1 == \n";
    cout << m1 << endl;

    cout << "m2 == \n";
    cout << m2 << endl;

    cout << "m1 * 2 ==\n";
    cout << m1 * 2 << endl;

    cout << "m1 * m2 ==\n";
    cout << m1 * m2 << endl;

    cout << "m2 * m1 ==\n";
    cout << m2 * m1 << endl;

    cout << "m2 ^ 2 ==\n";
    cout << (m2 ^ 2) << endl;

    fixed_matrix<double, 2, 2> m3({ 1, 2, 1, 2 }), m4({ 1, 1, 2, 2 });

    cout << "m3 == \n";
    cout << m3 << endl;

    cout << "m4 == \n";
    cout << m4 << endl;

    cout << "2 * m3 ==\n";
    cout << 2.0 * m3 << endl;

    cout << "m4 * m3 ==\n";
    cout << m4 * m3 << endl;

    cout << "m4 ^ 3 ==\n";
    cout << (m4 ^ 3) << endl;

    fixed_matrix<bigint10, 2, 2> fib({ 1, 1, 1, 0 });
    cout << "fib ^ 5 ==\n";
    cout << (fib ^ 5) << endl;
    cout << "fib ^ 50 ==\n";
    cout << (fib ^ 50) << endl;
    cout << "fib ^ 200 ==\n";
    cout << (fib ^ 200) << endl;
    cout << "fib ^ 400 ==\n";
    cout << (fib ^ 400) << endl;
    
    return 0;
}
