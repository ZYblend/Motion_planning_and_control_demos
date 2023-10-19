#include "plotting.h"
#include "common_datastruct.h"
#include "astar.h"

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
    loc2d start;
    start.x = 8;
    start.y = 9; 
 
    // Destination is the left-most top-most corner
    loc2d dest;
    dest.x = 0;
    dest.y = 0; 

    //Plotting plotter;
    //plotter.plot_start_dest(start,dest);
    //plotter.plot_grid(grid);
    //plt::plot({1,3,2,4});
    //plt::title("A*");
    //plt::axis("off");   
    //plt::show();
    //plt::detail::_interpreter::kill();
 
    AStar aStar(start, dest, grid);
    aStar.astarsearch2d();

    
 
    return 0;
}

