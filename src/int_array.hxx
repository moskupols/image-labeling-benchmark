#ifndef INT_ARRAY_HXX
#define INT_ARRAY_HXX

#include <vector>
#include <cstring>
#include <memory>

class IntVectorProvider
{
public:
    typedef std::vector<int> IntArray;

    static IntArray create(std::size_t size);
    static void destroy(IntArray &a);
    static void swap(IntArray &a, IntArray &b);
};

class UniqueIntArrayProvider
{
public:
    typedef std::unique_ptr<int[]> IntArray;

    static IntArray create(std::size_t size);
    static void destroy(IntArray &a);
    static void swap(IntArray &a, IntArray &b);
};

class IntArrayProvider
{
public:
    typedef int* IntArray;

    static IntArray create(std::size_t size);
    static void destroy(IntArray &a);
    static void swap(IntArray &a, IntArray &b);
};

#endif

