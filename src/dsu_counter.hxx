#ifndef DSU_COUNTER_HXX
#define DSU_COUNTER_HXX

#include "grid.hxx"
#include "dsu.hxx"

class DsuCellVisitor
{
public:
    DsuCellVisitor(std::size_t cols, int& answer, DisjointSetUnion &dsu):
        cols(cols),
        answer(answer),
        dsu(dsu)
    {}

    void operator()(std::size_t x, std::size_t y, std::size_t r, std::size_t c)
    {
        std::size_t v = x * cols + y;
        std::size_t u = r * cols + c;
        if (dsu.join(v, u))
            --answer;
    }

private:
    const std::size_t cols;
    int &answer;
    DisjointSetUnion &dsu;
};

template<template<class> class Grid=SimpleGrid>
class DsuCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        const Grid<Matrix> grid(m);

        int rows = grid.rows();
        int cols = grid.cols();

        DisjointSetUnion dsu(rows * cols);
        int answer = 0;

        DsuCellVisitor visit(cols, answer, dsu);

        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
            {
                if (m.getNumber(r, c))
                {
                    ++answer;
                    grid.forEachUpperNeighbor(r, c, visit);
                }
            }
        return answer;
    }
};

#endif

