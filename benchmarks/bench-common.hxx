#ifndef BENCH_COMMON_HXX
#define BENCH_COMMON_HXX

#include "benchmark/benchmark_api.h"

#include "profile_counter.hxx"
#include "dfs_counter.hxx"
#include "stack_dfs_counter.hxx"
#include "dsu_counter.hxx"
#include "twoline_dsu_counter.hxx"
#include "int_array.hxx"
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
    BENCHMARK_TEMPLATE(BM, DfsCounter<IntArrayProvider>, test); \
    BENCHMARK_TEMPLATE(BM, DfsCounter<UniqueIntArrayProvider>, test); \
    BENCHMARK_TEMPLATE(BM, DfsCounter<IntVectorProvider>, test); \
    BENCHMARK_TEMPLATE(BM, StackDfsCounter<StdIntStackFactory>, test); \
    BENCHMARK_TEMPLATE(BM, DsuCounter, test); \
    BENCHMARK_TEMPLATE(BM, TwolineDsuCounter, test);
   /*
    * BENCHMARK_TEMPLATE(BM, ProfileCounter<IntArrayProvider>, test); \
    * BENCHMARK_TEMPLATE(BM, ProfileCounter<UniqueIntArrayProvider>, test); \
    * BENCHMARK_TEMPLATE(BM, ProfileCounter<IntVectorProvider>, test);
    */

#define INSTANTIATE_TEST(test) \
    volatile test test##_singleton; \
    BENCH_ALL(test);

#endif

