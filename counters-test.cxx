#include <vector>

#include "gtest/gtest.h"

#include "matrix.h"
#include "counter.h"
#include "dfs_counter.h"

using namespace std;

// typed tests, see
// http://code.google.com/p/googletest/wiki/V1_7_AdvancedGuide#Typed_Tests

template<class C>
class CommonCounterTest : public testing::Test
{};

typedef testing::Types<
    ProfileCounter<>,
    DfsCounter<>
    > TESTED_COUNTERS;
TYPED_TEST_CASE(CommonCounterTest, TESTED_COUNTERS);

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

typedef DfsCounter<> ExemplaryCounter;

template<class C>
class ComparativeCounterTest : testing::Test
{
};

