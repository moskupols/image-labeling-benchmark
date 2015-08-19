#ifndef STACK_DFS_COUNTER
#define STACK_DFS_COUNTER

#include <stack>
#include <cstring>

#include "grid.hxx"

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
    StackDfsCellVisitor(Matrix &used, Stack &rowStack, Stack &colStack):
        used(used),
        rowStack(rowStack),
        colStack(colStack)
    {}

    void operator()(std::size_t, std::size_t, std::size_t r, std::size_t c)
    {
        if (used.getNumber(r, c))
            return;
        used.setNumber(r, c, 1);
        rowStack.push(r);
        colStack.push(c);
    }
private:
    Matrix &used;
    Stack &rowStack, &colStack;
};


template<
    class IntStackFactory=StdIntStackFactory,
    template<class> class Grid=SimpleGrid
>
class StackDfsCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix& m)
    {
        const Grid<Matrix> grid(m);

        int rows = grid.rows();
        int cols = grid.cols();

        auto rowStack = IntStackFactory::create(rows * cols);
        auto colStack = IntStackFactory::create(rows * cols);

        Matrix used(rows, cols, 0);

        StackDfsCellVisitor<Matrix, decltype(rowStack)>
            visit(used, rowStack, colStack);

        int answer = 0;
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (m.getNumber(r, c) && !used.getNumber(r, c))
                {
                    ++answer;

                    visit(0, 0, r, c);

                    while (!rowStack.empty())
                    {
                        int a = rowStack.top(), b = colStack.top();
                        rowStack.pop();
                        colStack.pop();

                        grid.forEachNeighbor(a, b, visit);
                    }
                }
        return answer;
    }
};

#endif

