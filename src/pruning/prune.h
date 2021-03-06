#ifndef PRUNE_H
#define PRUNE_H

#include "cube.h"
#include <vector>

class Prune {
protected:
    vector<char> prune_table;
    string file_path;

public:
    virtual int pruning_number(Cube &cube) = 0;
protected:
    void buildPruneTable(vector<vector<long long>> &transition_table, int state_count, int start_value = 0);
    int pruneTreeSearch(int state, vector<char> & table, char depth_left, char depth, int lastMove, vector<vector<long long>> &transition_table);
};


#endif //PRUNE_H

