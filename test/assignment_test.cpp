#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;

int run_test() {
    matrix<double> m1(3, 3, 0), m2(3, 3);

    cout << "m1 == \n";
    cout << m1 << endl;

    cout << "m1[1][2] == " << m1[1][2] << endl;

    m1[1][2] = 3;
    cout << "\nm1[1][2] := 3\n" << endl;

    cout << "m1 == \n";
    cout << m1 << endl;

    m2 = m1;
    m2.element(0, 0) = 1;
    cout << "\nm2 := m1\n";
    cout << "m2[0][0] := 1\n";
    cout << "const m3 := m2\n" << endl;

    const matrix<double> m3 = m2;
    cout << "m3 == \n";
    cout << m3 << endl;

    matrix<double> rect(2, 3, { { 1, 2, 3 }, { 4, 5, 6 } });
    cout << rect << rect.transpose() << endl;
    
    return 0;
}
