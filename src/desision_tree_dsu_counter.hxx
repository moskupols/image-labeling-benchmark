#ifndef DESISION_TREE_COUNTER_HXX
#define DESISION_TREE_COUNTER_HXX

#include "matrix.hxx"

template<template<class> class TempMatrix=ArrayMatrix>
class DesisionTreeDsuCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        int rows = m.getMatrixHeight();
        int cols = m.getMatrixWidth();

        TempMatrix<int> label(rows, cols, 0);
        int nextLabel = 1;

        for (int c = 0; c < cols; ++c)
        {
            if (m.getNumber(0, c))
            {
                int prev = (c ? label.getNumber(0, c-1) : 0);
                label.setNumber(0, c, prev ? prev : nextLabel++);
            }
        }

        int empty = 0;
        for (int r = 1; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
            {
                if (!m.getNumber(r, c))
                    continue;
                int& me = label.getNumber(r, c);
                int labelN = label.getNumber(r-1, c);
                if (labelN)
                    me = labelN;
                else
                {
                    int& labelNE = (c + 1 == cols ? empty : label.getNumber(r-1, c+1));
                    if (labelNE)
                    {
                        int& labelNW = (!c ? empty : label.getNumber(r-1, c-1));
                        if (labelNW)
                            copy3(me, labelNE, labelNW);
                        else
                        {
                            int& labelW = (!c ? empty : label.getNumber(r, c-1));
                            if (labelW)
                                copy3(me, labelNE, labelW);
                            else
                                me = labelNE;
                        }
                    }
                    else
                    {
                        int labelNW, labelW;
                        if (c && (labelNW = label.getNumber(r-1, c-1)))
                            me = labelNW;
                        else if (c && (labelW = label.getNumber(r, c-1)))
                            me = labelW;
                        else
                            me = nextLabel++;
                    }
                }
            }

        DisjointSetUnion dsu(nextLabel);
        bool met[nextLabel];
        memset(met, 0, sizeof met);
        int answer = 0;
        SimpleGrid<decltype(label)> grid(label);
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
            {
                int me = label.getNumber(r, c);
                if (me)
                {
                    if (!met[me])
                    {
                        ++answer;
                        met[me] = 1;
                    }
                    for (int i = 0; i < 4; ++i)
                    {
                        int nr = r + grid.DELTAS[i][0];
                        int nc = c + grid.DELTAS[i][1];
                        if (grid.validCoords(nr, nc))
                        {
                            int that = label.getNumber(nr, nc);
                            if (that && that != me)
                                answer -= dsu.join(me, that);
                        }
                    }
                }
            }

        return answer;
    }

protected:
    static void copy3(int& me, int& a, int& b)
    {
        a = b = me = std::min(a, b);
    }
};

#endif

