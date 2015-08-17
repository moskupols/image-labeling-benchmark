#include "matrix.h"

#include <cassert>
#include <algorithm>

VectorMatrix::VectorMatrix(int rows, int cols, int value):
    data(rows, vector<int>(cols, value))
{}

IntArrayMatrix::IntArrayMatrix(std::size_t rows, std::size_t cols, int value):
    height(rows),
    width(cols),
    data(rows && cols ? new int[rows * cols] : nullptr)
{
    std::fill(data, data + rows * cols, value);
}

IntArrayMatrix::IntArrayMatrix(const IntArrayMatrix::Vectors &v):
    height(v.size()),
    width(height ? v[0].size() : 0),
    data(height && width ? new int[height * width] : nullptr)
{
    for (std::size_t i = 0; i < height; ++i)
        copy(v[i].begin(), v[i].end(), data + i * width);
}

