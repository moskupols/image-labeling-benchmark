#include <vector>

#include "gtest/gtest.h"

#include "matrix.h"
#include "profile_counter.h"
#include "dfs_counter.h"
#include "dsu_counter.h"
#include "int_array.h"
#include "testgen.h"

using namespace std;

// typed tests, see
// http://code.google.com/p/googletest/wiki/V1_7_AdvancedGuide#Typed_Tests

typedef testing::Types<
    ProfileCounter<>, ProfileCounter<IntArrayProvider>,
    ProfileCounter<UniqueIntArrayProvider>,
    DfsCounter<>, DfsCounter<IntArrayProvider>,
    DfsCounter<UniqueIntArrayProvider>,
    DsuCounter>
        TestedCounters;
typedef DfsCounter<> ExemplaryCounter;


template<class C>
class CounterSanityTest : public testing::Test
{};
TYPED_TEST_CASE(CounterSanityTest, TestedCounters);

TYPED_TEST(CounterSanityTest, Trivial)
{
    for (int i = 0; i < 2; ++i)
    {
        vector<vector<int>> m = {{i}};
        VectorMatrix matrix(m);
        TypeParam counter;
        EXPECT_EQ(i, counter.getComponentsCount(matrix));
    }
}

TYPED_TEST(CounterSanityTest, Manual)
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
        EXPECT_EQ(t.second, counter.getComponentsCount(matrix));
    }
}

TYPED_TEST(CounterSanityTest, OneColumn)
{
    vector<vector<int>> m = {{0}, {1}, {0}, {1}};
    VectorMatrix matrix(m);
    TypeParam counter;
    EXPECT_EQ(2, counter.getComponentsCount(matrix));
}

TYPED_TEST(CounterSanityTest, OneRow)
{
    vector<vector<int>> m = {{0, 1, 0, 1}};
    VectorMatrix matrix(m);
    TypeParam counter;
    EXPECT_EQ(2, counter.getComponentsCount(matrix));
}


template<class TestedCounter>
class ComparativeCounterTest : public testing::Test
{
public:
    void randomComparativeTest(int seed, size_t maxSize)
    {
        SCOPED_TRACE(seed);
        ExemplaryCounter exemplary;
        TestedCounter tested;
        RandomMatrixGenerator<VectorMatrix> gen(seed);
        const VectorMatrix m = gen.nextNotLargerThan(maxSize);
        EXPECT_EQ(exemplary.getComponentsCount(m), tested.getComponentsCount(m));
    }

    void randomTestBatch(size_t count, size_t maxSize)
    {
        for (size_t seed = 0; seed < count; ++seed)
        {
            randomComparativeTest(seed, maxSize);
            if (HasFatalFailure())
                return;
        }
    }
};

template<>
void ComparativeCounterTest<ExemplaryCounter>::randomComparativeTest(int, size_t)
{}

template<>
void ComparativeCounterTest<ExemplaryCounter>::randomTestBatch(size_t, size_t)
{}

TYPED_TEST_CASE(ComparativeCounterTest, TestedCounters);

TYPED_TEST(ComparativeCounterTest, SmallRandom)
{
    EXPECT_NO_FATAL_FAILURE
    ({
        this->randomTestBatch(10000, 20);
    });
}

TYPED_TEST(ComparativeCounterTest, LargeRandom)
{
    EXPECT_NO_FATAL_FAILURE
    ({
        this->randomTestBatch(100, 500);
    });
}

TYPED_TEST(ComparativeCounterTest, HugeRandom)
{
    EXPECT_NO_FATAL_FAILURE
    ({
        this->randomTestBatch(20, 1000);
    });
}


