#include "plotting.h"

void Plotting::plot_grid(std::vector<std::vector<int>> grid){
    std::vector<double> vectorx,vectory;

    for(int i=0;i<grid.size();i++){
        for(int j=0;j<grid[0].size();j++){
            if(grid[i][j]==0)
                {   
                    vectorx.push_back(j+0.5);
                    vectory.push_back(-i-0.5);
                }
        }
    }
    plt::plot(vectorx,vectory,"sk");
}

void Plotting::plot_start_dest(loc2d start,loc2d dest){
    std::vector<double> start_x,start_y;
    start_x.push_back(start.y+0.5);
    start_y.push_back(-start.x-0.5);
    plt::plot(start_x,start_y,"bs");
    
    std::vector<double> dest_x,dest_y;
    dest_x.push_back(dest.y+0.5);
    dest_y.push_back(-dest.x-0.5);
    plt::plot(dest_x,dest_y,"gs");
}

void Plotting::plot_path(std::stack<loc2d> path){
    std::vector<double> pathx,pathy;
    while(!path.empty()){
            loc2d loc = path.top();
            path.pop();
            pathx.push_back(loc.y+0.5);
            pathy.push_back(-loc.x-0.5);
        }
    plt::plot(pathx, pathy, {{"color", "red"}});
}

void Plotting::show_image(std::vector<std::vector<int>> grid){
    plt::title("A*");
    plt::axis("off");
    for(int i{0}; i<=grid.size(); i++){
        plt::axhline(-i);
    }
    for(int i{0}; i<=grid.size(); i++){
        plt::axvline(i);
    }
    plt::show();
}