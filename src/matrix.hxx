#ifndef MATRIX_HXX
#define MATRIX_HXX

#include <vector>
#include <memory>
#include <cassert>

using std::vector;

template<typename T>
class VectorMatrix  // adapter over vector of vectors
{
public:
    typedef vector<vector<T>> Vectors;

    VectorMatrix() {}
    VectorMatrix(int rows, int cols, T value):
        data(rows, vector<T>(cols, value))
    {}
    explicit VectorMatrix(const Vectors &data): data(data) {}
    explicit VectorMatrix(Vectors &&data): data(data) {}

    template<class InputIt>
    VectorMatrix(int rows, int cols, InputIt begin, InputIt end):
        data(rows, vector<T>(cols))
    {
        for (auto& row : data)
            for (T& cell : row)
                cell = *begin++;
        assert(begin == end);
    }

    int getMatrixWidth() const { return data[0].size(); }
    int getMatrixHeight() const { return data.size(); }
    T getNumber(int row, int col) const { return data[row][col]; }
    T& getNumber(int row, int col) { return data[row][col]; }
    void setNumber(int row, int col, T value) { data[row][col] = value; }

private:
    Vectors data;
};

template<typename T>
class ArrayMatrix
{
public:
    typedef vector<vector<T>> Vectors;

    ArrayMatrix(): data(nullptr) {}

    ArrayMatrix(std::size_t rows, std::size_t cols, T value):
        height(rows),
        width(cols),
        data(rows && cols ? new T[rows * cols] : nullptr)
    {
        if (value)
            std::fill(data, data + rows * cols, value);
        else
            memset(data, 0, rows * cols * sizeof(T));
    }

    explicit ArrayMatrix(const Vectors &v):
        height(v.size()),
        width(height ? v[0].size() : 0),
        data(height && width ? new T[height * width] : nullptr)
    {
        for (std::size_t i = 0; i < height; ++i)
            copy(v[i].begin(), v[i].end(), data + i * width);
    }


    template<class InputIt>
    ArrayMatrix(std::size_t rows, std::size_t cols, InputIt begin, InputIt end):
        height(rows),
        width(cols),
        data(new T[rows * cols])
    {
        T* dataEnd = std::copy(begin, end, data);
        assert(dataEnd  == data + rows * cols);
    }

    ArrayMatrix(std::size_t rows, std::size_t cols, std::unique_ptr<T[]> &&data):
        height(rows),
        width(cols),
        data(data.release())
    {}

    ~ArrayMatrix() { delete[] data; }

    int getMatrixWidth() const { return width; }
    int getMatrixHeight() const { return height; }
    T getNumber(int row, int col) const { return data[row * width + col]; }
    T& getNumber(int row, int col) { return data[row * width + col]; }
    void setNumber(int row, int col, T value) { data[row * width + col] = value; }

private:
    std::size_t height, width;
    T *data;
};

using BinaryVectorMatrix = VectorMatrix<char>;

using BinaryArrayMatrix = ArrayMatrix<char>;
using BestBinaryMatrix = BinaryArrayMatrix;

#endif

