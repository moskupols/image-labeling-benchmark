#include <string>
#include <vector>

#include <cassert>

#include "CImg.h"

#include "img.hxx"


const char* imgDirPath = "assets/";
const char* imgOrder[] =
{
    "halfcircle.bmp",
    "lena.bmp",
    "codecomment.bmp",
    "SEO-Flow-Chart.bmp",
    "scheme.bmp"
};
const std::size_t imgCount = sizeof(imgOrder) / sizeof(*imgOrder);

std::vector<std::vector<char>> loadImageVectors(std::size_t imgId)
{
    assert(imgId < imgCount);
    std::string path = imgDirPath;
    path += imgOrder[imgId];

    cimg_library::CImg<unsigned char> image(path.c_str());
    std::vector<std::vector<char>> v(image.height(), std::vector<char>(image.width()));
    for (int i = 0; i < image.height(); ++i)
        for (int j = 0; j < image.width(); ++j)
            v[i][j] = image.atXY(j, i);
    return v;
}

