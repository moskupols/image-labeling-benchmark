#include "matrix.h"

VectorMatrix::VectorMatrix(int rows, int cols, int value):
    data(rows, vector<int>(cols, value))
{}

VectorMatrix::VectorMatrix(Vectors data):
    data(data)
{}

VectorMatrix::VectorMatrix(Vectors &&data):
    data(data)
{}

int VectorMatrix::getMatrixWidth() const
{
    return data[0].size();
}

int VectorMatrix::getMatrixHeight() const
{
    return data.size();
}

int VectorMatrix::getNumber(int row, int col) const
{
    return data[row][col];
}

