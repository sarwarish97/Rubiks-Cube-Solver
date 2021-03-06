#include "optimal_solver.h"
#include <iostream>
#include <iomanip>
#include "eoPruning.h"
#include "coPruning.h"
#include "cpPruning.h"
#include "cornerPruning.h"
#include "ep1Pruning.h"
#include "ep2Pruning.h"
#include "edgePruning.h"

double diffclock(clock_t clock1, clock_t clock2)
{
    double diffticks = abs(clock1 - clock2);
    double diffms = (diffticks) / (CLOCKS_PER_SEC / 1000);
    return diffms;
}

OptimalSolver::OptimalSolver()
{
    pruning.push_back(new eoPruning());
    pruning.push_back(new coPruning());
    pruning.push_back(new cpPruning());
    pruning.push_back(new cornerPruning());
    pruning.push_back(new ep1Pruning());
    pruning.push_back(new ep2Pruning());
    pruning.push_back(new edgePruning());
}

OptimalSolver::~OptimalSolver()
{
    for (auto pruning_method : pruning)
    {
        delete pruning_method;
    }
    pruning.clear();
}

char OptimalSolver::solve(string scramble)
{
    Cube cube(scramble);
    
    cout << "Solve the scramble \"" << scramble << "\": " << endl;
    char depth = IDA(cube);
    cout << endl;
    return depth;
}

char OptimalSolver::IDA(Cube &cube)
{
    solution = "";
    nodeCnt = 0;
    clock_t start = clock();

    cout << "Start solving" << endl;
    char depth;
    for (depth = 0; depth <= 20; depth++)
    {
        cout << "Depth " << setfill(' ') << setw(2) << (int)depth << ": " << flush;
        if (treeSearch(cube, depth, -1)) //reset cube???
        {
            cout << "solution, " << setfill(' ') << setw(10) << nodeCnt << " nodes visited" << endl;
            cout << solution.c_str() << endl;
            break;
        }
        else
        {
            cout << "nothing,  " << setfill(' ') << setw(10) << nodeCnt << " nodes visited" << endl;
        }
    }
    cout << "total: " << nodeCnt << " nodes visited" << endl;
    clock_t end = clock();
    double seconds = diffclock(start, end) / 1000.0;
    cout << "Time: " << seconds << " seconds(" << (int)(nodeCnt / seconds) << " nodes / second)" << endl;

    return depth;
}

bool OptimalSolver::treeSearch(Cube &cube, char depth, int lastMove)
{
    nodeCnt++;
    if (depth == 0)
    {
        return cube.is_solved();
    }
    else
    {
        for (auto pruning_method : pruning)
        {
            if (pruning_method->pruning_number(cube) > depth)
            {
                return false;
            }
        }

        for (int move = 0; move < 6; move++)
        {
            if (move == lastMove)
                continue;
            if ((lastMove & 1) == 1 && move == lastMove - 1)
                continue;

            for (int j = 0; j < 3; j++)
            {
                cube.apply_move(move);
                if (treeSearch(cube, depth - 1, move))
                {
                    solution = moveNames[move] + moveCntNames[j] + string(" ") + solution;
                    return true;
                }
            }
            cube.apply_move(move);
        }

        return false;
    }
}
