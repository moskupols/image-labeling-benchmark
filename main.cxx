#include <iostream>
#include <vector>

#include "counter.h"
#include "matrix.h"

using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;

    vector<vector<int>> v(n, vector<int>(m, 0));
    for (auto &r : v)
    {
        string s;
        cin >> s;
        for (size_t i = 0; i < s.size(); ++i)
            if (s[i] == '1')
                r[i] = 1;
    }

    VectorMatrix wrapped = VectorMatrix::wrapVectors(v);
    int ans = getNumberOfAllLevelComponentsBinary(wrapped);

    cout << ans << endl;

    return 0;
}

