#include "matrix.h"

Matrix Matrix::wrapVectors(VectorBasedMatrix &data)
{
    return Matrix(data);
}

int Matrix::getMatrixWidth() const
{
    return data[0].size();
}

int Matrix::getMatrixHeight() const
{
    return data.size();
}

int Matrix::getNumber(int row, int col) const
{
    return data[row][col];
}

