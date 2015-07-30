#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using std::vector;

class VectorMatrix  // adapter over vector of vectors
{
public:
    typedef vector<vector<int>> Vectors;

    static VectorMatrix wrapVectors(Vectors &data);

    int getMatrixWidth() const;
    int getMatrixHeight() const;
    int getNumber(int row, int col) const;

private:
    explicit VectorMatrix(Vectors &data):
        data(data)
    {}

    Vectors &data;
};

#endif

