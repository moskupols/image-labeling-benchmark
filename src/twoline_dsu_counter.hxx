#ifndef TWOLINE_DSU_COUNTER
#define TWOLINE_DSU_COUNTER

#include <cstring>

#include "counters_common.hxx"
#include "dsu.hxx"

class TwolineDsuCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        int rows = m.getMatrixHeight();
        int cols = m.getMatrixWidth();
        int maxId = (cols+1) / 2;

        DisjointSetUnion dsu = DisjointSetUnion(maxId+1);

        bool idUsed[maxId+1];
        memset(idUsed, 0, sizeof idUsed);

        int *mark[2] = {new int[cols], new int[cols]};
        memset(mark[0], 0, cols * sizeof(int));
        memset(mark[1], 0, cols * sizeof(int));

        int answer = 0;

        for (int r = 0; r < rows; ++r)
        {
            int numerator = 1;
            for (int c = 0; c < cols; ++c)
                if (m.getNumber(r, c))
                {
                    int& newMark = mark[1][c];
                    for (auto d : UPPER_DELTAS)
                    {
                        int newRId = 1 + d[0];
                        int newR = r + d[0], newC = c + d[1];
                        if (newR >= 0  // we don't look lower, so newR < rows
                                && newC >=0 && newC < cols
                                && mark[newRId][newC])  // mark != 0 <=> getNumber(r, c)
                        {
                            if (!newMark)
                                newMark = mark[newRId][newC];
                            else if (dsu.join(mark[newRId][newC], newMark))
                                --answer;
                        }
                    }
                    if (!newMark)
                    {
                        ++answer;
                        while (idUsed[numerator])
                            ++numerator;
                        newMark = numerator;
                        idUsed[numerator++] = true;
                    }
                }

            memset(idUsed, 0, sizeof(idUsed));
            for (int i = 0; i < cols; ++i)
                if (mark[1][i])
                    idUsed[mark[1][i] = dsu.getRepresentative(mark[1][i])] = true;
            dsu.reset();
            memset(mark[0], 0, cols * sizeof(int));
            std::swap(mark[0], mark[1]);
        }
        delete[] mark[0];
        delete[] mark[1];

        return answer;
    }
};

#endif

