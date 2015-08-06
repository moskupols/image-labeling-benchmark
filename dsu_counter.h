#ifndef DSU_COUNTER_H
#define DSU_COUNTER_H

#include "counters_common.h"
#include "dsu.h"

class DsuCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix &m)
    {
        int rows = m.getMatrixHeight();
        int cols = m.getMatrixWidth();
        DisjointSetUnion dsu(rows * cols);
    }
};

#endif

