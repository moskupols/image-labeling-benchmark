#include "bench-common.h"
#include "img/img.h"

using namespace cimg_library;
using namespace std;

template<class M, size_t IMG_ID>
using ImageSingleton =  Singleton<ImageLoader<M, IMG_ID>>;

#define INSTANTIATE_IMG_TEST(m, id) \
    typedef ImageSingleton<m, id> IMAGE_##m##_##id; \
    INSTANTIATE_TEST(IMAGE_##m##_##id);

INSTANTIATE_IMG_TEST(VectorMatrix, 0);
INSTANTIATE_IMG_TEST(VectorMatrix, 1);
INSTANTIATE_IMG_TEST(VectorMatrix, 2);
INSTANTIATE_IMG_TEST(VectorMatrix, 3);
INSTANTIATE_IMG_TEST(VectorMatrix, 4);

