#include <vector>

#include "gtest/gtest.h"

#include "matrix.hxx"
// #include "profile_counter.hxx"
#include "dfs_counter.hxx"
#include "stack_dfs_counter.hxx"
#include "dsu_counter.hxx"
#include "twoline_dsu_counter.hxx"
#include "utils/testgen.hxx"

using namespace std;

// typed tests, see
// http://code.google.com/p/googletest/wiki/V1_7_AdvancedGuide#Typed_Tests

typedef testing::Types<
    // ProfileCounter<>, ProfileCounter<IntArrayProvider>,
    // ProfileCounter<UniqueIntArrayProvider>,
    DfsCounter<Compressing2x2Grid>,
    StackDfsCounter<>,
    StackDfsCounter<StdIntStackFactory, Compressing2x2Grid>,
    DsuCounter<SimpleGrid>,
    InlinedDsuCounter<SimpleGrid>,
    DsuCounter<Compressing2x2Grid>,
    // InlinedDsuCounter<Compressing2x2Grid>,
    TwolineDsuCounter<Compressing2x2Grid>>
        TestedCounters;
typedef StackDfsCounter<StdIntStackFactory, Compressing2x2Grid> ExemplaryCounter;

typedef ArrayMatrix<char> Matrix;


template<class C>
class CounterSanityTest : public testing::Test
{};
TYPED_TEST_CASE(CounterSanityTest, TestedCounters);

TYPED_TEST(CounterSanityTest, Trivial)
{
    for (char i = 0; i < 2; ++i)
    {
        vector<vector<char>> m = {{i}};
        Matrix matrix(m);
        TypeParam counter;
        EXPECT_EQ(i, counter.getComponentsCount(matrix));
    }
}

TYPED_TEST(CounterSanityTest, Manual)
{
    pair<vector<vector<char>>, int> tests[] =
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
        Matrix matrix(t.first);
        TypeParam counter;
        EXPECT_EQ(t.second, counter.getComponentsCount(matrix));
    }
}

TYPED_TEST(CounterSanityTest, OneColumn)
{
    vector<vector<char>> m = {{0}, {1}, {0}, {1}};
    Matrix matrix(m);
    TypeParam counter;
    EXPECT_EQ(2, counter.getComponentsCount(matrix));
}

TYPED_TEST(CounterSanityTest, OneRow)
{
    vector<vector<char>> m = {{0, 1, 0, 1}};
    Matrix matrix(m);
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
        RandomMatrixGenerator<Matrix> gen(seed);
        const Matrix m = gen.nextNotLargerThan(maxSize);
        ASSERT_EQ(exemplary.getComponentsCount(m), tested.getComponentsCount(m));
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


