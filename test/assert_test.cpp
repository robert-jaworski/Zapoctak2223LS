#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;

int run_test() {
    const char* xyz = "XYZ";
    exceptions::assert_error e(xyz);
    cout << xyz << ' ' << e.msg() << endl;
    throw exceptions::assert_error("XYZ");
}
