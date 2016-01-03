#ifndef STACK_DFS_COUNTER
#define STACK_DFS_COUNTER

#include <stack>
#include <cstring>

#include "grid.hxx"
#include "colored_row.hxx"

class StdIntStackFactory
{
public:
    static std::stack<int> create(std::size_t)
    {
        return std::stack<int>();
    }
};

template<class Matrix, class Stack>
class StackDfsCellVisitor
{
public:
    StackDfsCellVisitor(Matrix &used, Stack &rowStack, Stack &colStack,
            ColoredRow &top, ColoredRow &bottom):
        used(used),
        rowStack(rowStack),
        colStack(colStack),
        top(top),
        bottom(bottom)
    {}

    void operator()(std::size_t, std::size_t, std::size_t r, std::size_t c)
    {
        if (used.getNumber(r, c))
            return;
        used.setNumber(r, c, 1);
        rowStack.push(r);
        colStack.push(c);
        if (r == 0)
            top[c] = color;
        if (r + 1 == (std::size_t)used.getMatrixHeight())
            bottom[c] = color;
    }

    int color = 0;

private:
    Matrix &used;
    Stack &rowStack, &colStack;
    ColoredRow &top, &bottom;
};


template<
    class IntStackFactory = StdIntStackFactory,
    class TempMatrix = BestMatrix>
class StackDfsCounter
{
public:
    template<class Grid>
    static int paint(const Grid& grid, ColoredRow &top, ColoredRow &bottom)
    {
        int rows = grid.rows();
        int cols = grid.cols();

        auto rowStack = IntStackFactory::create(rows * cols);
        auto colStack = IntStackFactory::create(rows * cols);

        TempMatrix used(rows, cols, 0);

        StackDfsCellVisitor<TempMatrix, decltype(rowStack)>
            visit(used, rowStack, colStack, top, bottom);

        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (grid.getColor(r, c) && !used.getNumber(r, c))
                {
                    ++visit.color;

                    visit(0, 0, r, c);

                    while (!rowStack.empty())
                    {
                        int a = rowStack.top(), b = colStack.top();
                        rowStack.pop();
                        colStack.pop();

                        grid.forEachNeighbor(a, b, visit);
                    }
                }
        return visit.color;
    }
};

#endif

