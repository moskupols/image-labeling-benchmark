#ifndef MATRIX_HXX
#define MATRIX_HXX

#include <vector>
#include <memory>
#include <cassert>

using std::vector;

class VectorMatrix  // adapter over vector of vectors
{
public:
    typedef vector<vector<int>> Vectors;

    VectorMatrix() {}
    VectorMatrix(int rows, int cols, int value = 0);
    explicit VectorMatrix(const Vectors &data): data(data) {}
    explicit VectorMatrix(Vectors &&data): data(data) {}

    template<class InputIt>
    VectorMatrix(int rows, int cols, InputIt begin, InputIt end):
        data(rows, vector<int>(cols))
    {
        for (auto& row : data)
            for (int& cell : row)
                cell = *begin++;
        assert(begin == end);
    }

    int getMatrixWidth() const { return data[0].size(); }
    int getMatrixHeight() const { return data.size(); }
    int getNumber(int row, int col) const { return data[row][col]; }
    void setNumber(int row, int col, int value) { data[row][col] = value; }

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

    template<class InputIt>
    IntArrayMatrix(std::size_t rows, std::size_t cols, InputIt begin, InputIt end):
        height(rows),
        width(cols),
        data(new int[rows * cols])
    {
        int* dataEnd = std::copy(begin, end, data);
        assert(dataEnd  == data + rows * cols);
    }

    IntArrayMatrix(std::size_t rows, std::size_t cols, std::unique_ptr<int> &&data);

    ~IntArrayMatrix() { delete[] data; }

    int getMatrixWidth() const { return width; }
    int getMatrixHeight() const { return height; }
    int getNumber(int row, int col) const { return data[row * width + col]; }
    void setNumber(int row, int col, int value) { data[row * width + col] = value; }

private:
    std::size_t height, width;
    int *data;
};

typedef IntArrayMatrix BestMatrix;

#endif

