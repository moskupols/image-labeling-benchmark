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
{}

DisjointSetUnion::~DisjointSetUnion()
{
    delete[] parent;
    delete[] rank;
}

DisjointSetUnion::elem_t DisjointSetUnion::join(elem_t a, elem_t b)
{
    a = getRepresentative(a);
    b = getRepresentative(b);
    if (a == b)
        return a;
    if (rank[a] < rank[b])  // rank heuristic
        std::swap(a, b);
    rank[a] += rank[b];
    return parent[b] = a;
}

DisjointSetUnion::elem_t DisjointSetUnion::getRepresentative(elem_t v) const
{
    if (parent[v] == FICTION)
        return v;
    else
        return parent[v] = getRepresentative(parent[v]);  // path compression
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

