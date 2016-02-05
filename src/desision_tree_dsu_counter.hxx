#ifndef DESISION_TREE_COUNTER_HXX
#define DESISION_TREE_COUNTER_HXX

#include "matrix.hxx"

class DesisionTreeDsuCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        int rows = m.getMatrixHeight();
        int cols = m.getMatrixWidth();

        DisjointSetUnion dsu(rows * cols);
        IdCalculator id(cols);
        int answer = 0;

        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
            {
                if (!m.getNumber(r, c))
                    continue;
                auto me = id(r, c);
                if (r && m.getNumber(r-1, c))
                    dsu.join(me, id(r-1, c));
                else if (r && c + 1 != cols && m.getNumber(r-1, c+1))
                {
                    if (c && m.getNumber(r-1, c-1))
                        answer -= dsu.join(me, id(r-1, c-1)) && dsu.join(id(r-1, c-1), id(r-1, c+1));
                    else if (c && m.getNumber(r, c-1))
                        answer -= dsu.join(me, id(r, c-1)) && dsu.join(id(r-1, c+1), id(r, c-1));
                    else
                        dsu.join(me, id(r-1, c+1));
                }
                else
                {
                    if (r && c && m.getNumber(r-1, c-1))
                        dsu.join(me, id(r-1, c-1));
                    else if (c && m.getNumber(r, c-1))
                        dsu.join(me, id(r, c-1));
                    else
                        ++answer;
                }
            }
        return answer;
    }
};

#endif

