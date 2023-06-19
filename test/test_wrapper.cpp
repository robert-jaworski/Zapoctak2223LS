#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;

extern int run_test();

int main() {
    try {
        run_test();
    } catch (const exceptions::assert_error& e) {
        cout << "Assertion failed: " << e.msg() << endl;
        return 1;
    } catch (const exception& e) {
        cout << "Unexpected error: " << e.what() << endl;
        return 2;
    }
    return 0;
}
