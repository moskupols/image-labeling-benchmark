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

class DsuPainter
{
public:
    template<class Grid>
    static int paint(const Grid &grid,
            ColoredRow &top, ColoredRow &bottom)
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
        paintRow(grid, 0, dsu, top);
        paintRow(grid, rows - 1, dsu, bottom);
        return answer;
    }

protected:
    template<class Grid>
    static void paintRow(const Grid &grid, int r,
            const DisjointSetUnion &dsu, ColoredRow &row)
    {
        int cols = grid.cols();
        int begin = r * cols;
        for (int c = 0; c < cols; ++c)
            if (grid.getColor(r, c))
                row[c] = dsu.getRepresentative(begin + c);
    }
};

#endif

