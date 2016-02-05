#ifndef GRID_HXX
#define GRID_HXX

#include <cstddef>
#include <vector>

#include "matrix.hxx"

template<class Matrix=BestBinaryMatrix>
class SimpleGrid
{
public:
    explicit SimpleGrid(const Matrix &m):
        m(m)
    {}

    inline std::size_t rows() const { return m.getMatrixHeight(); }
    std::size_t cols() const { return m.getMatrixWidth(); }

    inline bool isDense() const { return true; }

    inline int getColor(std::size_t row, std::size_t col) const
    {
        return m.getNumber(row, col);
    }

    inline bool validCoords(std::size_t row, std::size_t col) const
    {
        return row >= 0 && col >= 0 && row < rows() && col < cols(/*row*/);
    }

    template<class Action>
    inline void forEachNeighbor(std::size_t row, std::size_t col, Action action) const
    {
        for (int i = 0; i < 8; ++i)
            tryTouchNeighbor(row, col, action, i);
    }

    template<class Action>
    inline void forEachUpperNeighbor(std::size_t row, std::size_t col, Action action) const
    {
        for (int i = 0; i < 4; ++i)
            tryTouchNeighbor(row, col, action, i);
    }

    inline bool canGo(std::size_t row, std::size_t col, int dir) const
    {
        std::size_t nr = row + DELTAS[dir][0], nc = col + DELTAS[dir][1];
        return validCoords(nr, nc) && getColor(nr, nc);
    }

    static const int DELTAS[8][2];

protected:
    template<class Action>
    inline void tryTouchNeighbor(std::size_t row, std::size_t col, Action action, int dir) const
    {
        if (canGo(row, col, dir))
        {
            std::size_t nr = row + DELTAS[dir][0], nc = col + DELTAS[dir][1];
            action(row, col, nr, nc);
        }
    }

private:
    const Matrix &m;
};

template<class Matrix>
const int SimpleGrid<Matrix>::DELTAS[8][2] =
{
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},           {0, 1},
    {1, -1},  {1, 0},  {1, 1}
};


template<class GivenMatrix=BestBinaryMatrix>
class Compressing2x2Grid
{
public:
    explicit Compressing2x2Grid(const GivenMatrix &bitmap):
        m(compressBitmap(bitmap))
    {}

    inline std::size_t rows() const { return m.getMatrixHeight(); }
    inline std::size_t cols() const { return m.getMatrixWidth(); }

    inline bool isDense() const { return false; }

    inline int getColor(std::size_t row, std::size_t col) const
    {
        return m.getNumber(row, col);
    }

    inline bool validCoords(std::size_t row, std::size_t col) const
    {
        return row >= 0 && col >= 0 && row < rows() && col < cols(/*row*/);
    }

    template<class Action>
    inline void forEachNeighbor(std::size_t row, std::size_t col, Action action) const
    {
        for (int i = 0; i < 8; ++i)
            tryTouchNeighbor(row, col, action, i);
    }

    template<class Action>
    inline void forEachUpperNeighbor(std::size_t row, std::size_t col, Action action) const
    {
        for (int i = 0; i < 4; ++i)
            tryTouchNeighbor(row, col, action, i);
    }

    inline bool canGo(std::size_t row, std::size_t col, int dir) const
    {
        std::size_t nr = row + DELTAS[dir][0], nc = col + DELTAS[dir][1];
        return (getColor(row, col) & DELTA_MASKS[dir][0])
                && validCoords(nr, nc)
                && (getColor(nr, nc) & DELTA_MASKS[dir][1]);
    }

    static const int DELTAS[8][2];
    static const int DELTA_MASKS[8][2];

    static ArrayMatrix<char> compressBitmap(const GivenMatrix &bitmap)
    {
        std::size_t oldRows = bitmap.getMatrixHeight();
        std::size_t oldCols = bitmap.getMatrixWidth();
        std::size_t newRows = (oldRows + 1) / 2;
        std::size_t newCols = (oldCols + 1) / 2;

        std::unique_ptr<char[]> compressedOwned{new char[newRows * newCols]};
        char* compressIter = compressedOwned.get();
        for (std::size_t i = 0; i < oldRows; i += 2)
        {
            for (std::size_t j = 0; j < oldCols; j += 2)
                *(compressIter++) =
                    bitmap.getNumber(i, j)
                    | (j+1 < oldCols
                            ? bitmap.getNumber(i, j+1) << 1
                            : 0)
                    | (i+1 < oldRows
                            ? bitmap.getNumber(i+1, j) << 2
                            : 0)
                    | (i+1 < oldRows && j+1 < oldCols
                            ? bitmap.getNumber(i + 1, j + 1) << 3
                            : 0);
        }
        return ArrayMatrix<char>(newRows, newCols, std::move(compressedOwned));
    }

protected:
    template<class Action>
    inline void tryTouchNeighbor(std::size_t row, std::size_t col, Action action, int dir) const
    {
        if (canGo(row, col, dir))
        {
            std::size_t nr = row + DELTAS[dir][0], nc = col + DELTAS[dir][1];
            action(row, col, nr, nc);
        }
    }

private:
    const ArrayMatrix<char> m;
};

template<class GivenMatrix>
const int Compressing2x2Grid<GivenMatrix>::DELTAS[8][2] =
{
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},           {0, 1},
    {1, -1},  {1, 0},  {1, 1}
};

template<class GivenMatrix>
const int Compressing2x2Grid<GivenMatrix>::DELTA_MASKS[8][2] =
{
    {1,   8},  {1|2, 4|8}, {2,   4},
    {1|4, 2|8},            {2|8, 1|4},
    {4,   2},  {4|8, 1|2}, {8,   1}
};

#endif

