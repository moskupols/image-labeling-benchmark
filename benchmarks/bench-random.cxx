#include "bench-common.hxx"
#include "utils/testgen.hxx"

using namespace std;

template<class M, size_t ROWS, size_t COLS, int SEED, int DENSITY>
using StaticRandomTest =
    Singleton<RandomMatrixGeneratorFunctor<M, ROWS, COLS, SEED, DENSITY>>;

#define INSTANTIATE_RANDOM_TEST(m, r, c, s, d) \
    typedef StaticRandomTest<m, r, c, s, d> RANDOM_##m##_##r##x##c##_##s##_##d; \
    INSTANTIATE_TEST(RANDOM_##m##_##r##x##c##_##s##_##d);

#define INSTANTIATE_RANDOM_TEST_2_MATRICES(r, c, s, d) \
    INSTANTIATE_RANDOM_TEST(BinaryVectorMatrix, r, c, s, d); \
    INSTANTIATE_RANDOM_TEST(BinaryArrayMatrix, r, c, s, d);

#define INSTANTIATE_RANDOM_TEST_3_DENSITIES_2_MATRICES(r, c, s) \
    INSTANTIATE_RANDOM_TEST_2_MATRICES(r, c, s, 5); \
    INSTANTIATE_RANDOM_TEST_2_MATRICES(r, c, s, 10); \
    INSTANTIATE_RANDOM_TEST_2_MATRICES(r, c, s, 20);
    // INSTANTIATE_RANDOM_TEST_2_MATRICES(r, c, s, 50);
    // INSTANTIATE_RANDOM_TEST_2_MATRICES(r, c, s, 80);

INSTANTIATE_RANDOM_TEST_3_DENSITIES_2_MATRICES(200, 600, 0);
INSTANTIATE_RANDOM_TEST_3_DENSITIES_2_MATRICES(600, 200, 1);
INSTANTIATE_RANDOM_TEST_3_DENSITIES_2_MATRICES(500, 500, 2);
INSTANTIATE_RANDOM_TEST_3_DENSITIES_2_MATRICES(10, 10000, 3);
INSTANTIATE_RANDOM_TEST_3_DENSITIES_2_MATRICES(10000, 10, 4);

