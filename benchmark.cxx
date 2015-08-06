#include "benchmark/benchmark_api.h"

#include "profile_counter.h"
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

#define INSTANTIATE_RANDOM_TEST(m, r, c, s, d) \
    typedef StaticRandomTest<m, r, c, s, d> RANDOM_##m##_##r##x##c##_##s##_##d; \
    INSTANTIATE_TEST(RANDOM_##m##_##r##x##c##_##s##_##d);

#define INSTANTIATE_RANDOM_TEST_3_DENSITIES(m, r, c, s) \
    INSTANTIATE_RANDOM_TEST(m, r, c, s, 20); \
    INSTANTIATE_RANDOM_TEST(m, r, c, s, 50); \
    INSTANTIATE_RANDOM_TEST(m, r, c, s, 80);

INSTANTIATE_RANDOM_TEST_3_DENSITIES(VectorMatrix, 200, 600, 0);
INSTANTIATE_RANDOM_TEST_3_DENSITIES(VectorMatrix, 600, 200, 1);
INSTANTIATE_RANDOM_TEST_3_DENSITIES(VectorMatrix, 500, 500, 2);
INSTANTIATE_RANDOM_TEST_3_DENSITIES(VectorMatrix, 10, 10000, 3);
INSTANTIATE_RANDOM_TEST_3_DENSITIES(VectorMatrix, 10000, 10, 4);

BENCHMARK_MAIN();

