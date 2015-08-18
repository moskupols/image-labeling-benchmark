#ifndef GRID_HXX
#define GRID_HXX

#include <cstddef>

template<class Matrix>
class SimpleGrid
{
public:
    explicit SimpleGrid(const Matrix &m):
        m(m)
    {}

    std::size_t rows() const { return m.getMatrixHeight(); }
    std::size_t cols() const { return m.getMatrixWidth(); }
    // std::size_t cols(std::size_t [>row<]) const { return m.getMatrixWidth(); }

    int getColor(std::size_t row, std::size_t col) const
    {
        return m.getNumber(row, col);
    }

    bool validCoords(std::size_t row, std::size_t col) const
    {
        return row >= 0 && col >= 0 && row < rows() && col < cols(/*row*/);
    }

    template<class Action>
    void forEachNeighbor(std::size_t row, std::size_t col, Action action) const
    {
        for (int i = 0; i < 8; ++i)
            tryTouchNeighbor(row, col, action, i);
    }

    template<class Action>
    void forEachUpperNeighbor(std::size_t row, std::size_t col, Action action) const
    {
        for (int i = 0; i < 4; ++i)
            tryTouchNeighbor(row, col, action, i);
    }

protected:
    template<class Action>
    void tryTouchNeighbor(std::size_t row, std::size_t col, Action action, int dir) const
    {
        std::size_t nr = row + DELTAS[dir][0], nc = col + DELTAS[dir][1];
        if (validCoords(nr, nc) && getColor(nr, nc))
            action(row, col, nr, nc);
    }

    static const int DELTAS[8][2];

private:
    const Matrix &m;
};

template<class Matrix>
const int SimpleGrid<Matrix>::DELTAS[8][2] =
{
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1}
};

#endif

