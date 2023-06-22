#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;

int run_test() {
    fixed_matrix<int_finite_field<5>, 3, 3> m1({ 1, 1, 1, 0, 0, 1, 0, 1, 1 });
    fixed_matrix<long_finite_field<5>, 3, 4> m2({ 3, 0, 3, 0, 3, 3, 0, 0, 6, 3, 3, 0 });
    matrix<finite_field<int>> m3(3, 3, finite_field<int>(5, 0)), m4(3, 4, finite_field<int>(5, 0));
    m3 = { 1_Zp, 1_Zp, 1_Zp, 0_Zp, 1_Zp, 1_Zp, 0_Zp, 0_Zp, 1_Zp, };
    m4 = { 3_Zp, 0_Zp, 3_Zp, 0_Zp, 3_Zp, 3_Zp, 0_Zp, 0_Zp, 6_Zp, 3_Zp, 3_Zp, 0_Zp, };
    fixed_matrix<int_finite_field<5>, 3, 3> m5({ 0, 1, 0, 2, 0, 0, 2, 1, 1 });

    finite_field<int> x = 0_Zp, y = 1_Zp, z(5, 3);
    cout << x << endl;
    x = 1;
    cout << x << endl;
    x = y;
    cout << x << endl;
    x = z;
    cout << x << endl;

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
    cout << "m3 ==\n" << m3 << endl;
    cout << "REF(m3) ==\n" << m3.get_REF() << endl;
    cout << "RREF(m3) ==\n" << m3.get_RREF() << endl;
    cout << "rank(m4) == " << m4.compute_rank() << endl;
    cout << "m4 ==\n" << m4 << endl;
    cout << "REF(m4) ==\n" << m4.get_REF() << endl;
    cout << "RREF(m4) ==\n" << m4.get_RREF() << endl;
    cout << "m3 ^ 0 ==\n" << (m3 ^ 0) << endl;
    cout << "m3 ^ -1 ==\n" << (m3 ^ -1) << endl;
    cout << "m5 ==\n" << m5 << endl;
    cout << "det(m5) ==\n" << m5.compute_determinant_REF() << endl;

    return 0;
}
