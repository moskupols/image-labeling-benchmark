#include "benchmark/benchmark_api.h"

#include "counter.h"
#include "dfs_counter.h"
#include "matrix.h"
#include "testgen.h"

template<class Counter, class Matrix>
void BM_Random(benchmark::State& state)
{
    state.PauseTiming();
    Counter counter;
    RandomMatrixGenerator<Matrix> gen(state.range_x());
    Matrix m = gen.next(600, 400);
    state.ResumeTiming();
    while (state.KeepRunning())
    {
        counter.getComponentsCount(m);
    }
}

#define BENCH(counter, matrix) \
    BENCHMARK_TEMPLATE(BM_Random, counter, matrix) \
        ->Arg(0)->Arg(1)->Arg(2)->Arg(3)->Arg(4)

BENCH(DfsCounter<>, VectorMatrix);
BENCH(ProfileCounter<>, VectorMatrix);

BENCHMARK_MAIN();

