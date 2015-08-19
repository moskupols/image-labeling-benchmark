#ifndef DFS_COUNTER_HXX
#define DFS_COUNTER_HXX

#include "grid.hxx"

template<class Matrix, class Grid>
class DfsCellVisitor
{
public:
    DfsCellVisitor(Matrix &used, const Grid &grid):
        used(used),
        grid(grid)
    {}

    void operator()(std::size_t, std::size_t, std::size_t r, std::size_t c)
    {
        if (used.getNumber(r, c))
            return;
        used.setNumber(r, c, 1);
        grid.forEachNeighbor(r, c, *this);
    }

private:
    Matrix &used;
    const Grid &grid;
};

template<template<class> class Grid=SimpleGrid>
class DfsCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Grid<Matrix> &grid)
    {
        std::size_t rows = grid.rows(), cols = grid.cols();

        Matrix used(rows, cols, 0);
        DfsCellVisitor<Matrix, Grid<Matrix>> visit(used, grid);

        int ans = 0;
        for (std::size_t r = 0; r < rows; ++r)
            for (std::size_t c = 0; c < cols; ++c)
            {
                if (grid.getColor(r, c) && !used.getNumber(r, c))
                {
                    ++ans;
                    visit(0, 0, r, c);
                }
            }
        return ans;
    }

    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        return getComponentsCount(Grid<Matrix>(m));
    }
};

#endif

