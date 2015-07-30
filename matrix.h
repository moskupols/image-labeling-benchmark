#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using std::vector;

class Matrix  // adapter over vector of vectors
{
public:
    typedef vector<vector<int>> VectorBasedMatrix;

    static Matrix wrapVectors(VectorBasedMatrix &data);

    int getMatrixWidth() const;
    int getMatrixHeight() const;
    int getNumber(int row, int col) const;

private:
    explicit Matrix(VectorBasedMatrix &data):
        data(data)
    {}

    VectorBasedMatrix &data;
};

#endif

