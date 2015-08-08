#include "CImg.h"

#include "bench-common.h"
#include "img/order.h"

using namespace cimg_library;
using namespace std;

template<class M, size_t IMG_ID>
class StaticImageTest
{
public:
    typedef M Matrix;

    StaticImageTest():
        m(load())
    {
        assert(!singleton);
        singleton = this;
    }

    static const Matrix& get()
    {
        return singleton->m;
    }

protected:
    static StaticImageTest<Matrix, IMG_ID>* singleton;

    static Matrix load()
    {
        string path = "img/";
        path += imgOrder[IMG_ID];

        CImg<unsigned char> image(path.c_str());
        vector<vector<int>> v(image.height(), vector<int>(image.width()));
        for (int i = 0; i < image.height(); ++i)
            for (int j = 0; j < image.width(); ++j)
                v[i][j] = image.atXY(j, i);
        return Matrix(v);
    }

private:
    const Matrix m;
};

template<class M, size_t IMG_ID>
StaticImageTest<M, IMG_ID>*
StaticImageTest<M, IMG_ID>::singleton = nullptr;

#define INSTANTIATE_IMG_TEST(m, id) \
    typedef StaticImageTest<m, id> IMAGE_##m##_##id; \
    INSTANTIATE_TEST(IMAGE_##m##_##id);

INSTANTIATE_IMG_TEST(VectorMatrix, 0);
INSTANTIATE_IMG_TEST(VectorMatrix, 1);
INSTANTIATE_IMG_TEST(VectorMatrix, 2);
INSTANTIATE_IMG_TEST(VectorMatrix, 3);
INSTANTIATE_IMG_TEST(VectorMatrix, 4);

