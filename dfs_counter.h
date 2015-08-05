#ifndef DFS_COUNTER_H
#define DFS_COUNTER_H

#include <vector>
#include <algorithm>

using std::vector;

template<class Matrix, class IntArrayProvider>
class DfsCounterImpl
{
public:
    explicit DfsCounterImpl(const Matrix &matrix):
        rows(matrix.getMatrixHeight()),
        cols(matrix.getMatrixWidth()),
        matrix(matrix)
    {}

    int getComponentsCount() const
    {
        IntArrayProvider provider;
        used = provider.create(rows * cols);
        int ans = 0;
        timer = 0;
        for (int i = 0; i < rows * cols; ++i)
        {
            if (used[i])
                continue;
            int r = i / cols, c = i % cols;
            if (matrix.getNumber(r, c))
            {
                ++ans;
                paint(r, c);
            }
        }
        return ans;
    }

protected:
    static const int DELTAS[8][2];

    inline bool isOk(int r, int c) const
    {
        return r >= 0 && c >= 0 && r < rows && c < cols;
    }

    inline void paint(int r, int c) const
    {
        int v = r * cols + c;
        used[v] = 1;
        assert(timer++ < rows * cols);
        for (auto d : DELTAS)
        {
            int new_r = r + d[0], new_c = c + d[1];
            int u = v + d[0] * cols + d[1];
            if (isOk(new_r, new_c) && !used[u] && matrix.getNumber(new_r, new_c))
                paint(new_r, new_c);
        }
    }

private:
    mutable int timer;
    const int rows, cols;
    const Matrix &matrix;
    mutable typename IntArrayProvider::IntArray used;
};

template<class M, class I>
const int DfsCounterImpl<M, I>::DELTAS[8][2] =
    {
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, 1},
        {1, 1},
        {1, 0},
        {1, -1},
        {0, -1}
    };

template<class IntArrayProvider=IntVectorProvider>
class DfsCounter
{
public:
    template<class Matrix>
    int getComponentsCount(const Matrix &m) const
    {
        return DfsCounterImpl<Matrix, IntArrayProvider>(m).getComponentsCount();
    }
};

#endif

