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
    static int getComponentsCount(const Grid<Matrix> &grid)
    {
        int rows = grid.rows();
        int cols = grid.cols();

        DisjointSetUnion dsu(rows * cols);
        int answer = 0;

        DsuCellVisitor visit(cols, answer, dsu);

        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
            {
                if (grid.getColor(r, c))
                {
                    ++answer;
                    grid.forEachUpperNeighbor(r, c, visit);
                }
            }
        return answer;
    }

    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        return getComponentsCount(Grid<Matrix>(m));
    }
};

template<template<class> class Grid>
class InlinedDsuCounter;

template<>
class InlinedDsuCounter<SimpleGrid>
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix &matrix)
    {
        typedef SimpleGrid<Matrix> Grid;

        std::size_t rows = matrix.getMatrixHeight();
        std::size_t cols = matrix.getMatrixWidth();

        DisjointSetUnion dsu(rows * cols);
        int answer = 0;

        int v = 0;
        for (std::size_t r = 0; r < rows; ++r)
            for (std::size_t c = 0; c < cols; ++c)
            {
                if (matrix.getNumber(r, c))
                {
                    ++answer;
                    for (int i = 0; i < 4; ++i)
                    {
                        auto& d = Grid::DELTAS[i];
                        std::size_t nr = r + d[0], nc = c + d[1];
                        if (nr < rows && nc < cols && matrix.getNumber(nr, nc))
                        {
                            std::size_t u = v + d[0] * cols + d[1];
                            if (dsu.join(v, u))
                                --answer;
                        }
                    }
                }
                ++v;
            }
        return answer;
    }
};

#endif

