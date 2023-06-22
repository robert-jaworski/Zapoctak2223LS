#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;
using namespace number_utils;

int run_test() {
    matrix<double, 3, 3> m1({ 1, 2, 3, 4, 5, 6, 7, 8, 10 });
    matrix<fraction<int>, 3, 3> m2({ 1, 2, 3, 4, 5, 6, 7, 8, 10 });
    matrix<fraction<bigint10>, 3, 3> m3({ 1_BI10, 2_BI10, 3_BI10, 4_BI10, 5_BI10, 6_BI10, 7_BI10, 8_BI10, 10_BI10 });

    cout << "rank(m1) == " << m1.compute_rank() << endl;
    cout << "m1 ==\n" << m1 << endl;
    cout << "REF(m1) ==\n" << m1.get_REF() << endl;
    cout << "RREF(m1) ==\n" << m1.get_RREF() << endl;
    cout << "det(m1) == " << m1.compute_determinant_REF() << endl;
    cout << "m1 ^ -1 == \n" << (m1 ^ -1) << endl;
    cout << "rank(m2) == " << m2.compute_rank() << endl;
    cout << "m2 ==\n" << m2 << endl;
    cout << "REF(m2) ==\n" << m2.get_REF() << endl;
    cout << "RREF(m2) ==\n" << m2.get_RREF() << endl;
    cout << "det(m2) == " << m2.compute_determinant_REF() << endl;
    cout << "m2 ^ -1 == \n" << (m2 ^ -1) << endl;
    cout << "rank(m3) == " << m3.compute_rank() << endl;
    cout << "m3 ==\n" << m3 << endl;
    cout << "REF(m3) ==\n" << m3.get_REF() << endl;
    cout << "RREF(m3) ==\n" << m3.get_RREF() << endl;
    cout << "det(m3) == " << m3.compute_determinant_REF() << endl;
    cout << "m3 ^ -1 == \n" << (m3 ^ -1) << endl;

    fraction<int> a(1, 10);
    fraction<int> b(2, 10);
    fraction<int> c = a + b;
    cout << "0.1  + 0.2  ==         "        << setprecision(30) << (0.1 + 0.2) << endl;
    cout << "1/10 + 2/10 == " << c << " == " << setprecision(30) << c.value<double>() << endl;

    return 0;
}
