#ifndef DFS_COUNTER_H
#define DFS_COUNTER_H

#include "counters_common.h"
#include "int_array.h"

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
        provider.destroy(used);
        return ans;
    }

protected:
    inline bool isOk(int r, int c) const
    {
        return r >= 0 && c >= 0 && r < rows && c < cols;
    }

    inline void paint(int r, int c) const
    {
        int v = r * cols + c;
        used[v] = 1;
        for (auto d : DELTAS)
        {
            int newR = r + d[0], newC = c + d[1];
            int u = v + d[0] * cols + d[1];
            if (isOk(newR, newC) && !used[u] && matrix.getNumber(newR, newC))
                paint(newR, newC);
        }
    }

private:
    const int rows, cols;
    const Matrix &matrix;
    mutable typename IntArrayProvider::IntArray used;
};

template<class IntArrayProvider=IntVectorProvider>
class DfsCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        return DfsCounterImpl<Matrix, IntArrayProvider>(m).getComponentsCount();
    }
};

#endif

