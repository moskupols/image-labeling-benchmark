#ifndef DSU_H
#define DSU_H

#include <cstddef>

// DSU, as described p.e. at
// https://en.wikipedia.org/wiki/Disjoint-set_data_structure
// and in the book by T. Cormen et al.
class DisjointSetUnion
{
public:
    typedef std::size_t elem_t;

    static const elem_t FICTION;

    explicit DisjointSetUnion(std::size_t size);
    ~DisjointSetUnion();

    bool join(elem_t a, elem_t b);  // amortized O(\alpha(size))
    // where \alpha is the inverse Ackermann function (practically small constant)

    elem_t getRepresentative(elem_t v) const;  // amortized O(\alpha(size))

    void reset();

    void swap(DisjointSetUnion& that);

private:
    std::size_t size;
    mutable elem_t* parent;
    std::size_t* rank;
};

#endif

