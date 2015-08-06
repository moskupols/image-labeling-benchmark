#ifndef DSU_COUNTER_H
#define DSU_COUNTER_H

#include "counters_common.h"
#include "dsu.h"

class DsuCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        int rows = m.getMatrixHeight();
        int cols = m.getMatrixWidth();
        DisjointSetUnion dsu(rows * cols);
        int answer = 0;
        int v = 0;
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
            {
                if (m.getNumber(r, c))
                {
                    ++answer;
                    for (auto d : UPPER_DELTAS)
                    {
                        int newR = r + d[0], newC = c + d[1];
                        int u = v + d[0] * cols + d[1];
                        if (newR >= 0  // we don't look lower
                                && newC >=0 && newC < cols
                                && m.getNumber(newR, newC)
                                && dsu.join(v, u))
                            --answer;
                    }
                }
                ++v;
            }
        return answer;
    }
};

#endif

