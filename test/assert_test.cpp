#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;

int run_test() {
    dynamic_matrix<double> m1(2, 3), m2(3, 2);
    try {
        dynamic_matrix<double> m(0, -1);
        return 1;
    } catch (exceptions::assert_error& e) { }

    try {
        m1[4][4] = 1;
        return 1;
    } catch (exceptions::assert_error& e) { }

    try {
        m1 = m2;
        return 1;
    } catch (exceptions::assert_error& e) { }

    try {
        m1 = { 1, 2 };
        return 1;
    } catch (exceptions::assert_error& e) { }

    try {
        m1 = { { 1, 2 }, { 1, 2 } };
        return 1;
    } catch (exceptions::assert_error& e) { }

    try {
        auto m = m1 * m2;
        return 1;
    } catch (exceptions::assert_error& e) { }

    try {
        m1 ^= 2;
        return 1;
    } catch (exceptions::assert_error& e) { }

    try {
        m1.transpose_self();
        return 1;
    } catch (exceptions::assert_error& e) { }

    return 0;
}
