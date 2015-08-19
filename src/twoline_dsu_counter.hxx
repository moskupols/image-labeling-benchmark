#ifndef TWOLINE_DSU_COUNTER
#define TWOLINE_DSU_COUNTER

#include <cstring>

#include "grid.hxx"
#include "dsu.hxx"

class TwolineDsuCellVisitor
{
public:
    TwolineDsuCellVisitor(int &newMark, int* mark[2], int& answer, DisjointSetUnion& dsu):
        newMark(newMark),
        mark(mark),
        answer(answer),
        dsu(dsu)
    {}

    void operator()(std::size_t x, std::size_t, std::size_t r, std::size_t c)
    {
        if (!newMark)
            newMark = mark[1+r-x][c];
        else if (dsu.join(mark[1+r-x][c], newMark))
            --answer;
    }

private:
    int& newMark;
    int** mark;
    int& answer;
    DisjointSetUnion& dsu;
};

template<template<class> class Grid=SimpleGrid>
class TwolineDsuCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Grid<Matrix> &grid)
    {
        int rows = grid.rows();
        int cols = grid.cols();
        int maxId = (cols+1) / 2;

        DisjointSetUnion dsu = DisjointSetUnion(maxId+1);

        bool idUsed[maxId+1];
        memset(idUsed, 0, sizeof idUsed);

        int *mark[2] = {new int[cols], new int[cols]};
        memset(mark[0], 0, cols * sizeof(int));
        memset(mark[1], 0, cols * sizeof(int));

        int answer = 0;
        int newMark;

        TwolineDsuCellVisitor visit(newMark, mark, answer, dsu);

        for (int r = 0; r < rows; ++r)
        {
            int numerator = 1;
            for (int c = 0; c < cols; ++c)
                if (grid.getColor(r, c))
                {
                    newMark = 0;
                    grid.forEachUpperNeighbor(r, c, visit);
                    if (!newMark)
                    {
                        ++answer;
                        while (idUsed[numerator])
                            ++numerator;
                        newMark = numerator;
                        idUsed[numerator++] = true;
                    }
                    mark[1][c] = newMark;
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

    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        return getComponentsCount(Grid<Matrix>(m));
    }
};

#endif

