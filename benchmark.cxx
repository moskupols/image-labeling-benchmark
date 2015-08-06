#include "benchmark/benchmark_api.h"

#include "counter.h"
#include "dfs_counter.h"
#include "int_array.h"
#include "matrix.h"
#include "testgen.h"

using namespace std;

template<class M, size_t ROWS, size_t COLS, int SEED=0, int DENSITY=50>
class StaticRandomTest
{
public:
    typedef M Matrix;

    StaticRandomTest():
        m(gen())
    {
        assert(!singleton);
        singleton = this;
    }

    static const Matrix& get()
    {
        return singleton->m;
    }

protected:
    static StaticRandomTest<Matrix, ROWS, COLS, SEED, DENSITY>* singleton;

    static Matrix gen()
    {
        RandomMatrixGenerator<Matrix> g(SEED);
        return g.nextWithDensity(ROWS, COLS, DENSITY / 100.);
    }

private:
    const Matrix m;
};

template<class M, size_t ROWS, size_t COLS, int SEED, int DENSITY>
StaticRandomTest<M, ROWS, COLS, SEED, DENSITY>*
StaticRandomTest<M, ROWS, COLS, SEED, DENSITY>::singleton = nullptr;

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

#define BENCH_ALL(test) \
    BENCHMARK_TEMPLATE(BM, DfsCounter<IntArrayProvider>, test); \
    BENCHMARK_TEMPLATE(BM, DfsCounter<UniqueIntArrayProvider>, test); \
    BENCHMARK_TEMPLATE(BM, DfsCounter<IntVectorProvider>, test); \
    BENCHMARK_TEMPLATE(BM, ProfileCounter<IntArrayProvider>, test); \
    BENCHMARK_TEMPLATE(BM, ProfileCounter<UniqueIntArrayProvider>, test); \
    BENCHMARK_TEMPLATE(BM, ProfileCounter<IntVectorProvider>, test);

#define INSTANTIATE_TEST(test) \
    volatile test test##_singleton; \
    BENCH_ALL(test);

#define INSTANTIATE_TEST_RANDOM(m, r, c, s, d) \
    typedef StaticRandomTest<m, r, c, s, d> RANDOM_##m##_##r##x##c##_##s##_##d; \
    INSTANTIATE_TEST(RANDOM_##m##_##r##x##c##_##s##_##d);

INSTANTIATE_TEST_RANDOM(VectorMatrix, 200, 600, 0, 20);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 600, 200, 1, 20);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 500, 500, 2, 20);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 10, 10000, 3, 20);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 10000, 10, 4, 20);

INSTANTIATE_TEST_RANDOM(VectorMatrix, 200, 600, 0, 50);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 600, 200, 1, 50);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 500, 500, 2, 50);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 10, 10000, 3, 50);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 10000, 10, 4, 50);

INSTANTIATE_TEST_RANDOM(VectorMatrix, 200, 600, 5, 80);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 600, 200, 6, 80);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 500, 500, 7, 80);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 10, 10000, 8, 80);
INSTANTIATE_TEST_RANDOM(VectorMatrix, 10000, 10, 9, 80);

BENCHMARK_MAIN();

