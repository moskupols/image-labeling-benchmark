#include <vector>

#include "gtest/gtest.h"

#include "matrix.h"
#include "counter.h"
#include "dfs_counter.h"

using namespace std;

// typed tests, see
// http://code.google.com/p/googletest/wiki/V1_7_AdvancedGuide#Typed_Tests

template<class C>
class CounterTestFixture : public testing::Test
{
};

typedef testing::Types<
    ProfileCounter<VectorMatrix>,
    DfsCounter<VectorMatrix>
    > TESTED_COUNTERS;
TYPED_TEST_CASE(CounterTestFixture, TESTED_COUNTERS);

TYPED_TEST(CounterTestFixture, Trivial)
{
    for (int i = 0; i < 2; ++i)
    {
        vector<vector<int>> m = {{i}};
        VectorMatrix matrix = VectorMatrix::wrapVectors(m);
        TypeParam counter(matrix);
        ASSERT_EQ(i, counter.getComponentsCount());
    }
}

TYPED_TEST(CounterTestFixture, Manual)
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
            {1, 1, 0, 1, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 0},
            {1, 0, 1, 0, 1, 0, 1},
            {1, 1, 1, 0, 1, 1, 1}
        }, 5}
    };
    for (auto t : tests)
    {
        VectorMatrix matrix = VectorMatrix::wrapVectors(t.first);
        TypeParam counter(matrix);
        ASSERT_EQ(t.second, counter.getComponentsCount());
    }
}

