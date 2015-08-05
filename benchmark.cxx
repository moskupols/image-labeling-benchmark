#include "benchmark/benchmark_api.h"

#include "counter.h"
#include "dfs_counter.h"
#include "matrix.h"
#include "testgen.h"

using namespace std;

template<class M, size_t ROWS, size_t COLS, int SEED=0>
class StaticRandomTest
{
public:
    typedef M Matrix;

    StaticRandomTest():
        m(gen(ROWS, COLS, SEED))
    {
        assert(!singleton);
        singleton = this;
    }

    static const Matrix& get()
    {
        return singleton->m;
    }

protected:
    static StaticRandomTest<Matrix, ROWS, COLS, SEED>* singleton;

    static Matrix gen(size_t rows, size_t cols, int seed)
    {
        RandomMatrixGenerator<Matrix> g(seed);
        return g.next(rows, cols);
    }

private:
    const Matrix m;
};

template<class M, size_t ROWS, size_t COLS, int SEED>
StaticRandomTest<M, ROWS, COLS, SEED>*
StaticRandomTest<M, ROWS, COLS, SEED>::singleton = nullptr;

template<class Counter, class Matrix>
void runTest(benchmark::State& state, const Counter& counter, const Matrix& m)
{
    while (state.KeepRunning())
    {
        counter.getComponentsCount(m);
    }
}

template<class Counter, class StaticTest>
void BM(benchmark::State& state)
{
    Counter counter;
    const typename StaticTest::Matrix& m = StaticTest::get();
    runTest(state, counter, m);
}

#define INSTANTIATE_TEST(m, r, c, s) \
    typedef StaticRandomTest<m, r, c, s> m##_##r##x##c##_##s; \
    volatile StaticRandomTest<m, r, c, s> m##_##r##x##c##_##s##_singleton; \
    BENCHMARK_TEMPLATE(BM, DfsCounter<>, m##_##r##x##c##_##s); \
    BENCHMARK_TEMPLATE(BM, ProfileCounter<>, m##_##r##x##c##_##s)

#define INSTANTIATE_VECTOR_TEST(r, c, s) \
    INSTANTIATE_TEST(VectorMatrix, r, c, s)

INSTANTIATE_VECTOR_TEST(200, 600, 0);
INSTANTIATE_VECTOR_TEST(600, 200, 1);
INSTANTIATE_VECTOR_TEST(500, 500, 2);
INSTANTIATE_VECTOR_TEST(10, 10000, 3);
INSTANTIATE_VECTOR_TEST(10000, 10, 4);


BENCHMARK_MAIN();

