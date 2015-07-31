#include <vector>

#include "gtest/gtest.h"

#include "matrix.h"
#include "counter.h"
#include "dfs_counter.h"
#include "testgen.h"

using namespace std;

// typed tests, see
// http://code.google.com/p/googletest/wiki/V1_7_AdvancedGuide#Typed_Tests

template<class C>
class CommonCounterTest : public testing::Test
{};

typedef testing::Types<
    ProfileCounter<>,
    DfsCounter<>
    > TestedCounters;
TYPED_TEST_CASE(CommonCounterTest, TestedCounters);

TYPED_TEST(CommonCounterTest, Trivial)
{
    for (int i = 0; i < 2; ++i)
    {
        vector<vector<int>> m = {{i}};
        VectorMatrix matrix(m);
        TypeParam counter;
        ASSERT_EQ(i, counter.getComponentsCount(matrix));
    }
}

TYPED_TEST(CommonCounterTest, Manual)
{
    pair<vector<vector<int>>, int> tests[] =
    {
        {{
            {1, 0},
            {0, 1}
        }, 1},
        {{
            {1, 0, 1},
            {1, 0, 1},
            {1, 1, 1}
        }, 1},
        {{
            {1, 0, 1},
            {0, 1, 0},
            {1, 0, 1}
        }, 1},
        {{
             {1, 0, 1}
        }, 2},
        {{
             {1, 1, 0, 0, 1},
             {1, 0, 0, 1, 1}
        }, 2},
        {{
            {1, 1, 0, 1, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 0},
            {1, 0, 1, 0, 1, 0, 1},
            {1, 1, 1, 0, 1, 1, 1}
        }, 5}
    };
    for (auto t : tests)
    {
        VectorMatrix matrix(t.first);
        TypeParam counter;
        ASSERT_EQ(t.second, counter.getComponentsCount(matrix));
    }
}


template<class C>
class ComparativeCounterTest : public testing::Test
{};

typedef DfsCounter<> ExemplaryCounter;
typedef testing::Types<ProfileCounter<>> ComparativeTestedCounters;
TYPED_TEST_CASE(ComparativeCounterTest, ComparativeTestedCounters);

TYPED_TEST(ComparativeCounterTest, SmallRandom)
{
    ExemplaryCounter exemplary;
    TypeParam tested;
    for (int seed = 0; seed < 500; ++seed)
    {
        RandomMatrixGenerator<VectorMatrix> gen(seed);
        VectorMatrix m = gen.nextNotLargerThan(20);
        ASSERT_EQ(exemplary.getComponentsCount(m), tested.getComponentsCount(m));
    }
}

