#ifndef IMG_HXX
#define IMG_HXX

#include <vector>

#include <cstddef>

extern const char* imgDirPath;
extern const char* imgOrder[];
extern const std::size_t imgCount;

std::vector<std::vector<int>> loadImageVectors(size_t imgId);

template<class Matrix>
Matrix loadImage(std::size_t imgId)
{
    return Matrix(loadImageVectors(imgId));
}

template<class Matrix, std::size_t IMG_ID>
struct ImageLoader
{
    Matrix operator()() const
    {
        return loadImage<Matrix>(IMG_ID);
    }
};

#endif

