#ifndef MATRIX_HXX
#define MATRIX_HXX

#include <vector>

using std::vector;

class VectorMatrix  // adapter over vector of vectors
{
public:
    typedef vector<vector<int>> Vectors;

    VectorMatrix() {}
    VectorMatrix(int rows, int cols, int value = 0);
    explicit VectorMatrix(const Vectors &data): data(data) {}
    explicit VectorMatrix(Vectors &&data): data(data) {}

    int getMatrixWidth() const { return data[0].size(); }
    int getMatrixHeight() const { return data.size(); }
    int getNumber(int row, int col) const { return data[row][col]; }

private:
    Vectors data;
};

class IntArrayMatrix
{
public:
    typedef vector<vector<int>> Vectors;

    IntArrayMatrix(): data(nullptr) {}
    IntArrayMatrix(std::size_t rows, std::size_t cols, int value = 0);
    explicit IntArrayMatrix(const Vectors &data);

    ~IntArrayMatrix() { delete[] data; }

    int getMatrixWidth() const { return width; }
    int getMatrixHeight() const { return height; }
    int getNumber(int row, int col) const { return data[row * width + col]; }

private:
    std::size_t height, width;
    int *data;
};

#endif

