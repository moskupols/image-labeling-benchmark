#include "matrix.h"

VectorMatrix VectorMatrix::wrapVectors(Vectors &data)
{
    return VectorMatrix(data);
}

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

