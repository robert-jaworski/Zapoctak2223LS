#include <bits/stdc++.h>
#include "../src/all.hpp"

using namespace std;
using namespace matrices;

int run_test() {
    vector<dynamic_matrix<double>> m = {
        {2, 2, { 1, 2, 1, 2 }},
        {2, 2, { {1, 2}, {1, 2} }},
        {2, 2, 0},
        {3, 3, { 1, 1, 1, 2, 2, 2, 3, 3, 3 }}
    };
    
    vector<bool> RESULTS = {
        true, true, false, false,
        true, true, false, false,
        false, false, true, false,
        false, false, false, true
    };

    int fail = 0;
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m.size(); j++) {
            bool res = m[i] == m[j];
            if (res != RESULTS[i * m.size() + j])
                fail++;
            cout << "m[" << i << "] " << (res ? "==" : "!=") << " m[" << j << "]\n";
        }
    }
    
    return fail;
}
