#ifndef INT_ARRAY_H
#define INT_ARRAY_H

#include <vector>
#include <cstring>
#include <memory>

class IntVectorProvider
{
public:
    typedef std::vector<int> IntArray;

    static IntArray create(std::size_t size);
    static void swap(IntArray &a, IntArray &b);
};

class IntArrayProvider
{
public:
    typedef std::unique_ptr<int[]> IntArray;

    static IntArray create(std::size_t size);
    static void swap(IntArray &a, IntArray &b);
};

#endif

