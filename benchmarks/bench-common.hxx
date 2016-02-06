#ifndef BENCH_COMMON_HXX
#define BENCH_COMMON_HXX

#include <benchmark/benchmark_api.h>

#include "dfs_counter.hxx"
#include "stack_dfs_counter.hxx"
#include "dsu_counter.hxx"
#include "twoline_dsu_counter.hxx"
#include "profile_counter.hxx"
#include "desision_tree_dsu_counter.hxx"
#include "grid.hxx"
#include "matrix.hxx"

#include "utils/singleton.hxx"

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
    const auto& m = StaticTest::get();
    runTest(state, counter, m);
}

#define BENCH_ALL(test) \
    /* BENCHMARK_TEMPLATE(BM, ProfileCounter, test); */ \
    BENCHMARK_TEMPLATE(BM, DfsCounter<>, test); \
    BENCHMARK_TEMPLATE(BM, DfsCounter<Compressing2x2Grid>, test); \
    BENCHMARK_TEMPLATE(BM, DfsCounter<Viewing2x2Grid>, test); \
    /* BENCHMARK_TEMPLATE(BM, StackDfsCounter<>, test); */ \
    /* BENCHMARK_TEMPLATE(BM, StackDfsCounter<StdIntStackFactory, Compressing2x2Grid>, test); */ \
    BENCHMARK_TEMPLATE(BM, DsuCounter<>, test); \
    BENCHMARK_TEMPLATE(BM, DsuCounter<Compressing2x2Grid>, test); \
    BENCHMARK_TEMPLATE(BM, DsuCounter<Viewing2x2Grid>, test); \
    BENCHMARK_TEMPLATE(BM, DesisionTreeDsuCounter, test); \
    BENCHMARK_TEMPLATE(BM, TwolineDsuCounter<>, test); \
    BENCHMARK_TEMPLATE(BM, TwolineDsuCounter<Viewing2x2Grid>, test); \
    BENCHMARK_TEMPLATE(BM, TwolineDsuCounter<Compressing2x2Grid>, test);

#define INSTANTIATE_TEST(test) \
    volatile test test##_singleton; \
    BENCH_ALL(test);

#endif

