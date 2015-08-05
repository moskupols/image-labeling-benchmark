#include <vector>
#include <memory>
#include <cstring>

#include "int_array.h"

using namespace std;

IntVectorProvider::IntArray IntVectorProvider::create(size_t size)
{
    return IntArray(size);
}

void IntVectorProvider::swap(IntArray &a, IntArray &b)
{
    return a.swap(b);
}


IntArrayProvider::IntArray IntArrayProvider::create(std::size_t size)
{
    IntArray ret(new int[size]);
    memset(ret.get(), 0, size * sizeof(int));
    return ret;
}

void IntArrayProvider::swap(IntArray &a, IntArray &b)
{
    return a.swap(b);
}

