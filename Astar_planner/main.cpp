#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <stack>
#include <cfloat>

/*
A* algorithm 
Author: Yu Zheng, Florida State University

Using Eclidean Heristic to find the shortest path

cost (move from q to the next location p):
successor.g: q.g + qp.g (distance between the q to successor p)
successor.h: heristic distance from successor p to the destination
successor.c = successor.g + successor.h


1. initialize open list (priority queue)
              cloase list (hash table)
              predecessors ()
    Put start point to the open list, call it q
while (open list is not empty):
    2. pop q off open list, generate all successors
    3. for each successor:
       (1) if successor is outside boundary, or encounter blocks skip it
       (2) if successor is goal, stop search, extract path
       (3) if successor is in closed list, skip it
       (4) else: compute c, g, h for successor p
                (5) if successor is in open list:
                    (5.1) if open[p].cost > q.g + qp.g + p.h, 
                           open[p].cost = q.g + qp.g + p.h
                           assign q as parents to p
                    (5.2) else: skip
                (6) else: 
                        open.push(p, q.g+qp.g + p.h)
                        p.c = q.g + qp.g + p.h
                        assign q as parents to p
        (7) closed.push(q)
    end(for)
end(while)
*/

struct pPair{
    double cost; 
    int i,j;  // location
};
struct Comparecost { // use for ordering queue by the value of cost in ascending order
    bool operator()(pPair const& p1, pPair const& p2){
        return p1.cost > p2.cost;
    }
};
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // c = g + h
    double c, g, h;
};

class AStar {
public:
    void astarsearch2d(const std::vector<std::vector<int>> grid, const std::pair<int,int> start, 
                     const std::pair<int,int> dest){
        /* Input: 
                grid: map graph
                start: start location
                dest:  destination
        */
        int row = grid.size();
        int col = grid[0].size();
        if(outsidegrid(grid,start) || outsidegrid(grid,dest) || blocked(grid,start) || blocked(grid,dest)){
            std::cout << "either start location or destination is outside map or blocked" << std::endl;
            return;
        }
        
        // initialize closed list as 2d boolean vector
        std::vector<std::vector<bool>> closedList(row, std::vector<bool>(col, false));

        // initialize predecessors 2d
        cell initial;
        initial.parent_i = -1;
        initial.parent_j = -1;
        initial.c = FLT_MAX;
        initial.g = FLT_MAX;
        initial.h = FLT_MAX;
        std::vector<std::vector<cell>> predecessors(row, std::vector<cell>(col, initial));

        // initialize for the start location
        int i = start.first, j = start.second;
        predecessors[i][j].parent_i = i;
        predecessors[i][j].parent_j = j;
        predecessors[i][j].c = 0.0;
        predecessors[i][j].g = 0.0;
        predecessors[i][j].h = 0.0;

        // initialize open list
        std::priority_queue< pPair, std::vector<pPair>, Comparecost > openList; // minheap of {c,i,j}, ordered by c
        pPair curloc;
        curloc.cost = 0.0;
        curloc.i = i;
        curloc.j = j;
        openList.push(curloc); // put start location into open list

        bool founddest = false;

        while(!openList.empty()){
            // pop the node with minimal cost
            curloc = openList.top();
            openList.pop();
            closedList[curloc.i][curloc.j] = true;

            // get all successors on 8 directions
            //0: North (i-1, j) 1: South (i+1, j) 2: East (i, j+1) 3: West (i, j-1) 
            // 4:North-East (i-1, j+1) 5:North-West  (i-1, j-1)
            // 6:South-East  (i+1, j+1) 7: South-West  (i+1, j-1)
            double cnew, gnew, hnew;
            double gadd;
            for(int k=0; k<8; k++){
                if(k<4){gadd = 1;}else{gadd = 1.414;}
                std::pair<int,int> sucloc = std::make_pair(curloc.i+move[k][0], curloc.j+move[k][1]);
                if(!outsidegrid(grid,sucloc) && !blocked(grid,sucloc)){
                    if( sucloc.first == dest.first && sucloc.second == dest.second ){
                        predecessors[sucloc.first][sucloc.second].parent_i = curloc.i;
                        predecessors[sucloc.first][sucloc.second].parent_j = curloc.j;
                        std::cout << "Arrive!" << std::endl;
                        tracePath(predecessors, dest);
                        founddest = true;
                        return;
                    }
                    if(!closedList[sucloc.first][sucloc.second]){
                        gnew = predecessors[curloc.i][curloc.j].g + gadd;
                        hnew = euclideanHeuristic(sucloc, dest);
                        cnew = gnew+hnew;

                        // if it is not in openlist or
                        // it is in openlist, but cost is bigger than cnew
                        // put it in open list, and update its predecessor info
                        if(predecessors[sucloc.first][sucloc.second].c == FLT_MAX || predecessors[sucloc.first][sucloc.second].c>cnew ){
                            pPair loc;
                            loc.cost = cnew;
                            loc.i = sucloc.first;
                            loc.j = sucloc.second;
                            openList.push(loc);

                            predecessors[sucloc.first][sucloc.second].parent_i = curloc.i;
                            predecessors[sucloc.first][sucloc.second].parent_j = curloc.j;
                            predecessors[sucloc.first][sucloc.second].c = cnew;
                            predecessors[sucloc.first][sucloc.second].g = gnew;
                            predecessors[sucloc.first][sucloc.second].h = hnew;
                        }
                    }
                }
                
            }
        }
        if(!founddest){
                std::cout << "Failed to find a path to Destination" << std::endl;
        }
    }
private:

    bool outsidegrid(const std::vector<std::vector<int>> grid, const std::pair<int,int> loc){
        int row = grid.size()-1;
        int col = grid[0].size()-1;
        if(loc.first>row || loc.first<0 || loc.second>col || loc.second<0){
            return true;
        }
        return false;
    }

    bool blocked(const std::vector<std::vector<int>> grid, const std::pair<int,int> loc){
        if(grid[loc.first][loc.second]==0){
            return true;
        }
        return false;
    }

    void tracePath(std::vector<std::vector<cell>> predecessors, std::pair<int,int> dest){
        std::cout << "The path is " << std::endl;
        int i = dest.first, j = dest.second;

        std::stack<std::pair<int,int>> path; // last in first out


        while (!(predecessors[i][j].parent_i == i && predecessors[i][j].parent_j == j)){
            path.push(std::make_pair(i,j));
            int temp_i = predecessors[i][j].parent_i;
            int temp_j = predecessors[i][j].parent_j;
            i = temp_i;
            j = temp_j;
        }

        // also push start location in
        path.push(std::make_pair(i,j));

        // print path
        while(!path.empty()){
            std::pair<int, int> loc = path.top();
            path.pop();
            std::cout << "( " << loc.first << " , " << loc.second << " )" << std::endl;
        }
    }

    double euclideanHeuristic(std::pair<int, int> loc, std::pair<int,int> dest){
        return sqrt(pow(dest.first-loc.first,2)+pow(dest.second-loc.second,2));
    }

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


};

int main()
{
    /* Description of the Grid-
     1--> The cell is not blocked
     0--> The cell is blocked    */
    std::vector<std::vector<int>> grid
        = { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
            { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
            { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
            { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
            { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
            { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } };
 
    // Source is the left-most bottom-most corner
    std::pair<int,int> start = std::make_pair(8, 0);
 
    // Destination is the left-most top-most corner
    std::pair<int,int> dest = std::make_pair(0, 0);
 
    AStar aStar;
    
    aStar.astarsearch2d(grid, start, dest);
 
    return (0);
}

