#ifndef IMG_H
#define IMG_H

#include <vector>
#include <string>

#include <cassert>
#include <cstddef>

#include "CImg.h"

extern const char* imgDirPath;
extern const char* imgOrder[];
extern const std::size_t imgCount;

template<class Matrix>
Matrix loadImage(size_t imgId)
{
    assert(imgId < imgCount);
    std::string path = imgDirPath;
    path += imgOrder[imgId];

    cimg_library::CImg<unsigned char> image(path.c_str());
    std::vector<std::vector<int>> v(image.height(), std::vector<int>(image.width()));
    for (int i = 0; i < image.height(); ++i)
        for (int j = 0; j < image.width(); ++j)
            v[i][j] = image.atXY(j, i);
    return Matrix(v);
}

template<class Matrix, size_t IMG_ID>
struct ImageLoader
{
    Matrix operator()() const
    {
        return loadImage<Matrix>(IMG_ID);
    }
};

#endif

