#include <iostream>
#include <vector>

#include "counter.h"
#include "dfs_counter.h"
#include "matrix.h"
#include "testgen.h"

using namespace std;

int main()
{
    // int n, m;
    // cin >> n >> m;

    // vector<vector<int>> v(n, vector<int>(m, 0));
    // for (auto &r : v)
    // {
        // string s;
        // cin >> s;
        // for (size_t i = 0; i < s.size(); ++i)
            // if (s[i] == '1')
                // r[i] = 1;
    // }

    RandomMatrixGenerator<VectorMatrix> gen(227);
    VectorMatrix m = gen.nextNotLargerThan(20);

    cout << m.getMatrixHeight() << ' ' << m.getMatrixWidth() << endl;
    for (int i = 0; i < m.getMatrixHeight(); ++i)
    {
        for (int j = 0; j < m.getMatrixWidth(); ++j)
            cout << ".#"[m.getNumber(i, j)];
        cout << "\n";
    }

    ProfileCounter<> prof;
    cout << prof.getComponentsCount(m) << endl;

    DfsCounter<> dfs;
    cout << dfs.getComponentsCount(m) << endl;

    return 0;
}

