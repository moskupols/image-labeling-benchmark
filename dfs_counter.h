#ifndef DFS_COUNTER_H
#define DFS_COUNTER_H

#include <vector>
#include <algorithm>

template<class Matrix, class IntArray=std::vector<int>>
class DfsCounter
{
public:
    explicit DfsCounter(const Matrix &matrix):
        rows(matrix.getMatrixHeight()),
        cols(matrix.getMatrixWidth()),
        matrix(matrix),
        used(rows * cols)
    {}

    int getComponentsCount() const
    {
        std::fill(std::begin(used), std::end(used), 0);
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
        return ans;
    }

protected:
    static const int DELTAS[8][2];

    bool isOk(int r, int c) const
    {
        return r >= 0 && c >= 0 && r < rows && c < cols;
    }

    void paint(int r, int c) const
    {
        int v = r * cols + c;
        used[v] = 1;
        for (auto d : DELTAS)
        {
            int new_r = r + d[0], new_c = c + d[1];
            int u = v + d[0] * cols + d[1];
            if (isOk(new_r, new_c) && !used[u] && matrix.getNumber(new_r, new_c))
                paint(new_r, new_c);
        }
    }

private:
    const int rows, cols;
    const Matrix &matrix;
    mutable IntArray used;
};

template<class M, class I>
const int DfsCounter<M, I>::DELTAS[8][2] =
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

#endif

