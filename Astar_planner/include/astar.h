#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <stack>
#include <cfloat>
#include "plotting.h"
#include "common_datastruct.h"

// Data structure for Open list
struct pPair{
    double cost; 
    int i,j;  // location
};
struct Comparecost { // use for ordering queue by the value of cost in ascending order // miniheap
    bool operator()(pPair const& p1, pPair const& p2){
        return p1.cost > p2.cost;
    }
};

// Data structure for predecessors
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // c = g + h
    double c, g, h;
};

class AStar {

public:
    AStar(loc2d start_loc, loc2d destination, std::vector<std::vector<int>> map);

    void astarsearch2d();

private:
    bool outsidegrid(loc2d loc);

    bool blocked(loc2d loc);

    void tracePath(std::vector<std::vector<cell>> predecessors);

    double euclideanHeuristic(loc2d loc);
    /*
    8 directions

        N.W   N   N.E
        \   |   /
            \  |  /
        W----Cell----E
            / | \
            /   |  \
        S.W    S   S.E

    Cell-->Popped Cell (i, j)
    N -->  North       (i-1, j)
    S -->  South       (i+1, j)
    E -->  East        (i, j+1)
    W -->  West           (i, j-1)
    N.E--> North-East  (i-1, j+1)
    N.W--> North-West  (i-1, j-1)
    S.E--> South-East  (i+1, j+1)
    S.W--> South-West  (i+1, j-1)*/
    std::vector<std::vector<int>> move {
        {-1, 0},
        {1, 0},
        {0, 1},
        {0, -1},
        {-1, 1},
        {-1, -1},
        {1, 1},
        {1, -1}
    };

    loc2d start;
    loc2d dest;
    std::vector<std::vector<int>> grid;
};


#endif