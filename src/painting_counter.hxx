#ifndef PAINTING_COUNTER_HXX
#define PAINTING_COUNTER_HXX

#include "colored_row.hxx"
#include "grid.hxx"

template<class Painter, template<class> class Grid>
class PaintingCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Grid<Matrix> &grid)
    {
        ColoredRow top(grid.cols());
        ColoredRow bottom(top);
        return Painter().paint(grid, top, bottom);
    }

    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        return getComponentsCount(Grid<Matrix>(m));
    }
};

#endif

