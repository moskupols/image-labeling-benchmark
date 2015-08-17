#ifndef STACK_DFS_COUNTER
#define STACK_DFS_COUNTER

#include <stack>
#include <cstring>

#include "counters_common.hxx"

class StdIntStackFactory
{
public:
    static std::stack<int> create(std::size_t)
    {
        return std::stack<int>();
    }
};

template<class IntStackFactory=StdIntStackFactory>
class StackDfsCounter
{
public:
    template<class Matrix>
    static int getComponentsCount(const Matrix& m)
    {
        int rows = m.getMatrixHeight();
        int cols = m.getMatrixWidth();

        auto rowStack = IntStackFactory::create(rows * cols);
        auto colStack = IntStackFactory::create(rows * cols);

        bool used[rows][cols];
        memset(used, 0, sizeof used);

        int answer = 0;
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (m.getNumber(r, c) && !used[r][c])
                {
                    ++answer;

                    rowStack.push(r);
                    colStack.push(c);
                    used[r][c] = true;

                    while (!rowStack.empty())
                    {
                        int a = rowStack.top(), b = colStack.top();
                        rowStack.pop();
                        colStack.pop();

                        for (auto d : DELTAS)
                        {
                            int newA = a + d[0], newB = b + d[1];
                            if (newA >= 0 && newA < rows
                                    && newB >= 0 && newB < cols
                                    && !used[newA][newB]
                                    && m.getNumber(newA, newB))
                            {
                                used[newA][newB] = true;
                                rowStack.push(newA);
                                colStack.push(newB);
                            }
                        }
                    }
                }
        return answer;
    }
};

#endif

