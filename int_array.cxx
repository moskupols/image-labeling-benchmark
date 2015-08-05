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

void IntVectorProvider::destroy(IntArray &a)
{
    a.clear();
}

UniqueIntArrayProvider::IntArray UniqueIntArrayProvider::create(std::size_t size)
{
    IntArray ret(new int[size]);
    memset(ret.get(), 0, size * sizeof(int));
    return ret;
}

void UniqueIntArrayProvider::destroy(IntArray &a)
{
    a.reset();
}

void UniqueIntArrayProvider::swap(IntArray &a, IntArray &b)
{
    return a.swap(b);
}

IntArrayProvider::IntArray IntArrayProvider::create(size_t size)
{
    int* ret = new int[size];
    memset(ret, 0, size * sizeof(int));
    return ret;
}

void IntArrayProvider::destroy(IntArray &a)
{
    delete[] a;
    a = nullptr;
}

void IntArrayProvider::swap(IntArray &a, IntArray &b)
{
    std::swap(a, b);
}

