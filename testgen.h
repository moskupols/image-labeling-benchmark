#ifndef TESTGEN_H
#define TESTGEN_H

#include <random>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cassert>

using std::vector;
using std::begin;
using std::end;
using std::shuffle;

template<class Matrix>
class RandomMatrixGenerator
{
public:
    explicit RandomMatrixGenerator(int seed):
        rng(seed)
    {}

    void setSeed(int newSeed)
    {
        rng = std::mt19937(newSeed);
    }

    Matrix next(size_t rows, size_t cols)
    {
        return nextWithOnes(rows, cols, rng() % (rows * cols + 1));
    }

    Matrix nextWithOnes(size_t rows, size_t cols, size_t ones)
    {
        size_t size = rows * cols;
        assert(ones <= size);

        char perm[size];
        memset(perm, 0, size);
        memset(perm, 1, ones);
        shuffle(perm, perm + size, rng);

        vector<vector<int>> res(rows, vector<int>(cols));
        for (int i = 0; i < size; ++i)
            res[i/cols][i % cols] = perm[i];

        return Matrix(res);
    }

    Matrix nextNotLargerThan(size_t n)
    {
        size_t rows = unsigned(rng()) % n + 1;
        size_t cols = unsigned(rng()) % n + 1;
        return next(rows, cols);
    }

private:
    std::mt19937 rng;
};

#endif

