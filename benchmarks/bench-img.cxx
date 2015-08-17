#include "bench-common.h"
#include "utils/img.h"

using namespace cimg_library;
using namespace std;

template<class M, size_t IMG_ID>
using ImageSingleton =  Singleton<ImageLoader<M, IMG_ID>>;

#define INSTANTIATE_IMG_TEST(m, id) \
    typedef ImageSingleton<m, id> IMAGE_##m##_##id; \
    INSTANTIATE_TEST(IMAGE_##m##_##id);

#define INSTANTIATE_IMG_TEST_2_MATRICES(id) \
    INSTANTIATE_IMG_TEST(VectorMatrix, id); \
    INSTANTIATE_IMG_TEST(IntArrayMatrix, id);

INSTANTIATE_IMG_TEST_2_MATRICES(0);
INSTANTIATE_IMG_TEST_2_MATRICES(1);
INSTANTIATE_IMG_TEST_2_MATRICES(2);
INSTANTIATE_IMG_TEST_2_MATRICES(3);
INSTANTIATE_IMG_TEST_2_MATRICES(4);

