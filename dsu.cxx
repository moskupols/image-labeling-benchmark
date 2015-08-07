#include "dsu.h"

#include <cstring>
#include <algorithm>

using namespace std;

const DisjointSetUnion::elem_t DisjointSetUnion::FICTION =
    ~((DisjointSetUnion::elem_t) 0); // unsigned 0xFFF...

DisjointSetUnion::DisjointSetUnion(size_t size):
    size(size),
    parent(new elem_t[size]),
    rank(new size_t[size])
{
    reset();
}

DisjointSetUnion::~DisjointSetUnion()
{
    delete[] parent;
    delete[] rank;
}

bool DisjointSetUnion::join(elem_t a, elem_t b)
{
    a = getRepresentative(a);
    b = getRepresentative(b);
    if (a == b)
        return false;
    if (rank[a] < rank[b])  // rank heuristic
        std::swap(a, b);
    rank[a] += rank[b];
    parent[b] = a;
    return true;
}

DisjointSetUnion::elem_t DisjointSetUnion::getRepresentative(elem_t v) const
{
    if (parent[v] == FICTION)
        return v;
    else
        return parent[v] = getRepresentative(parent[v]);  // path compression

    // Following non-recursive code is the fastest I can write, but still
    // a little slower than that recursive beauty
    //
    // static elem_t st[10];
    // if (parent[v] == FICTION)
        // return v;
    // int sz = 0;
    // st[sz++] = v;
    // v = parent[v];
    // while (parent[v] != FICTION)
    // {
        // st[sz++] = v;
        // v = parent[v];
    // }
    // elem_t ret = v;
    // for (int i = 0; i < sz; ++i)
        // parent[st[i]] = ret;
    // return ret;
}

void DisjointSetUnion::reset()
{
    memset(parent, -1, size * sizeof(*parent));
    fill(rank, rank + size, 1);
}

void DisjointSetUnion::swap(DisjointSetUnion& that)
{
    std::swap(size, that.size);
    std::swap(parent, that.parent);
    std::swap(rank, that.rank);
}

