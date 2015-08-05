#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using std::vector;

class VectorMatrix  // adapter over vector of vectors
{
public:
    typedef vector<vector<int>> Vectors;

    VectorMatrix();
    VectorMatrix(int rows, int cols, int value = 0);
    explicit VectorMatrix(Vectors data);
    explicit VectorMatrix(Vectors &&data);

    int getMatrixWidth() const;
    int getMatrixHeight() const;
    int getNumber(int row, int col) const;

private:
    Vectors data;
};

#endif

