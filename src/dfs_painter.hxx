#ifndef DFS_COUNTER_HXX
#define DFS_COUNTER_HXX

#include "grid.hxx"
#include "colored_row.hxx"

template<class Matrix, class Grid>
class DfsCellVisitor
{
public:
    DfsCellVisitor(Matrix &used, const Grid &grid, ColoredRow &top, ColoredRow &bottom):
        used(used),
        grid(grid),
        top(top),
        bottom(bottom)
    {}

    void operator()(std::size_t, std::size_t, std::size_t r, std::size_t c)
    {
        if (used.getNumber(r, c))
            return;
        used.setNumber(r, c, 1);
        if (r == 0)
            top[c] = color;
        if (r + 1 == grid.rows())
            bottom[c] = color;

        grid.forEachNeighbor(r, c, *this);
    }

    int color = 0;

private:
    Matrix &used;
    const Grid &grid;
    ColoredRow &top;
    ColoredRow &bottom;
};

template<class TempMatrix = BestMatrix>
class DfsPainter
{
public:
    template<class Grid>
    static int paint(const Grid &grid, ColoredRow &top, ColoredRow &bottom)
    {
        std::size_t rows = grid.rows(), cols = grid.cols();

        TempMatrix used(rows, cols, 0);
        DfsCellVisitor<TempMatrix, Grid> visit(used, grid, top, bottom);

        for (std::size_t r = 0; r < rows; ++r)
            for (std::size_t c = 0; c < cols; ++c)
            {
                if (grid.getColor(r, c) && !used.getNumber(r, c))
                {
                    ++visit.color;
                    visit(0, 0, r, c);
                }
            }
        return visit.color;
    }
};

#endif
