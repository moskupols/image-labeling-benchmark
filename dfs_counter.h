#ifndef DFS_COUNTER_H
#define DFS_COUNTER_H

#include <vector>
#include <algorithm>

using std::vector;

template<class Matrix, class IntArray=vector<int>>
class DfsCounter
{
public:
    explicit DfsCounter(const Matrix &matrix):
        rows(matrix.getMatrixHeight()),
        cols(matrix.getMatrixWidth()),
        matrix(matrix),
        used(matrix.getMatrixHeight() * matrix.getMatrixWidth()),
        deltas{-cols-1, -cols, -cols+1, 1, cols+1, cols, cols-1, -1}
    {}

    int getComponentsCount()
    {
        std::fill(std::begin(used), std::end(used), 0);
        int ans = 0;
        for (int i = 0; i < rows * cols; ++i)
            if (matrix.getNumber(i / cols, i % cols) && !used[i])
                paint(i);
        return ans;
    }

protected:
    bool isOk(int at)
    {
        return at >= 0 && at < cols * rows;
    }

    void paint(int v)
    {
        used[v] = 1;
        for (int d : deltas)
        {
            int u = v + d;
            if (isOk(u) && matrix.getNumber(u / cols, u % cols))
                paint(u);
        }
    }

private:
    int rows, cols;
    const Matrix &matrix;
    IntArray used;
    const int deltas[8];
};

#endif

