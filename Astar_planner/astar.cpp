#include "astar.h"

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
    Put start loc2d to the open list, call it q
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

AStar::AStar(loc2d start_loc, loc2d destination, std::vector<std::vector<int>> map){
    start = start_loc;
    dest = destination;
    grid = map;

    if(outsidegrid(start) || outsidegrid(dest) || blocked(start) || blocked(dest)){
        std::cout << "ERROR: either start location or destination is outside map or blocked" << std::endl;
        return;
    }
}

void AStar::astarsearch2d(){
    /* Input: 
            grid: map graph
            start: start location
            dest:  destination
    */
    int row = grid.size();
    int col = grid[0].size();
    
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
    int i = start.x, j = start.y;
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
            if(k<4){gadd = 1;}else{gadd = 1.414;}  // the length of the blocks are assumed to be 1
            loc2d sucloc;
            sucloc.x = curloc.i+move[k][0];
            sucloc.y = curloc.j+move[k][1];

            if(!outsidegrid(sucloc) && !blocked(sucloc)){
                if( sucloc.x == dest.x && sucloc.y == dest.y ){
                    predecessors[sucloc.x][sucloc.y].parent_i = curloc.i;
                    predecessors[sucloc.x][sucloc.y].parent_j = curloc.j;
                    std::cout << "Arrive!" << std::endl;
                    tracePath(predecessors);
                    founddest = true;
                    return;
                }
                if(!closedList[sucloc.x][sucloc.y]){
                    gnew = predecessors[curloc.i][curloc.j].g + gadd;
                    hnew = euclideanHeuristic(sucloc);
                    cnew = gnew+hnew;

                    // if it is not in openlist or
                    // it is in openlist, but cost is bigger than cnew
                    // put it in open list, and update its predecessor info
                    if(predecessors[sucloc.x][sucloc.y].c == FLT_MAX || predecessors[sucloc.x][sucloc.y].c>cnew ){
                        pPair loc;
                        loc.cost = cnew;
                        loc.i = sucloc.x;
                        loc.j = sucloc.y;
                        openList.push(loc);

                        predecessors[sucloc.x][sucloc.y].parent_i = curloc.i;
                        predecessors[sucloc.x][sucloc.y].parent_j = curloc.j;
                        predecessors[sucloc.x][sucloc.y].c = cnew;
                        predecessors[sucloc.x][sucloc.y].g = gnew;
                        predecessors[sucloc.x][sucloc.y].h = hnew;
                    }
                }
            }
            
        }
    }
    if(!founddest){
            std::cout << "Failed to find a path to Destination" << std::endl;
    }
}

bool AStar::outsidegrid(loc2d loc){
    int row = grid.size()-1;
    int col = grid[0].size()-1;
    if(loc.x>row || loc.x<0 || loc.y>col || loc.y<0){
        return true;
    }
    return false;
}

bool AStar::blocked(loc2d loc){
    if(grid[loc.x][loc.y]==0){
        return true;
    }
    return false;
}

void AStar::tracePath(std::vector<std::vector<cell>> predecessors){
    std::cout << "The path is " << std::endl;
    int i = dest.x, j = dest.y;

    std::stack<loc2d> path; // last in first out


    while (!(predecessors[i][j].parent_i == i && predecessors[i][j].parent_j == j)){
        loc2d path_loc;
        path_loc.x = i;
        path_loc.y = j;
        path.push(path_loc);
        int temp_i = predecessors[i][j].parent_i;
        int temp_j = predecessors[i][j].parent_j;
        i = temp_i;
        j = temp_j;
    }

    // also push start location in
    loc2d start_loc;
    start_loc.x = i;
    start_loc.y = j;
    path.push(start_loc);

    // plot path
    Plotting plotter;
    plotter.plot_start_dest(start,dest);
    plotter.plot_grid(grid);
    plotter.plot_path(path);
    plotter.show_image(grid);

    /* // print path
    while(!path.empty()){
        loc2d loc = path.top();
        path.pop();
        std::cout << "( " << loc.x << " , " << loc.y << " )" << std::endl;
    } */

}

double AStar::euclideanHeuristic(loc2d loc){
    return sqrt(pow(dest.x-loc.x,2)+pow(dest.y-loc.y,2));
}