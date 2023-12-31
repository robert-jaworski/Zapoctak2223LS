#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;

int run_test() {
    dynamic_matrix<double> m1(3, 3, { 1, 2, 3, 1, 2, 3, 1, 2, 3 }), m2(3, 3, { 1, 1, 1, 2, 2, 2, 3, 3, 3 });

    cout << "m1 == \n";
    cout << m1 << endl;

    cout << "m2 == \n";
    cout << m2 << endl;

    cout << "m1 + m2 ==\n";
    cout << m1 + m2 << endl;

    matrix<double, 2, 2> m3({ 1, 2, 1, 2 }), m4({ 1, 1, 2, 2 });

    cout << "m3 == \n";
    cout << m3 << endl;

    cout << "m4 == \n";
    cout << m4 << endl;

    cout << "m3 + m4 ==\n";
    cout << m3 + m4 << endl;
    
    return 0;
}
