#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;

int run_test() {
    dynamic_matrix<double> m1(3, 3, { 1, 1, 1, 0, 0, 1, 0, 1, 1 }), m2(3, 4, { 3, 0, 3, 0, 3, 3, 0, 0, 6, 3, 3, 0 });
    matrix<double, 3, 3> m3({ 1, 1, 1, 0, 1, 1, 0, 0, 1 });
    matrix<double, 3, 4> m4({ 3, 0, 3, 0, 3, 3, 0, 0, 6, 3, 3, 0 });
    matrix<double, 3, 3> m5({ 0, 1, 0, 2, 0, 0, 2, 1, 1 });

    cout << "rank(m1) == " << m1.compute_rank() << endl;
    cout << "m1 ==\n" << m1 << endl;
    cout << "REF(m1) ==\n" << m1.get_REF() << endl;
    cout << "RREF(m1) ==\n" << m1.get_RREF() << endl;
    cout << "rank(m2) == " << m2.compute_rank() << endl;
    cout << "m2 ==\n" << m2 << endl;
    cout << "REF(m2) ==\n" << m2.get_REF() << endl;
    cout << "RREF(m2) ==\n" << m2.get_RREF() << endl;
    cout << "m1 ^ -1 ==\n" << (m1 ^ -1) << endl;
    cout << "rank(m3) == " << m3.compute_rank() << endl;
    cout << "RREF(m3) ==\n" << m3.get_RREF() << endl;
    cout << "RREF(m4) ==\n" << m4.get_RREF() << endl;
    cout << "m3 ^ -1 ==\n" << (m3 ^ -1) << endl;
    cout << "m5 ==\n" << m5 << endl;
    cout << "det(m5) ==\n" << m5.compute_determinant_REF() << endl;

    return 0;
}
