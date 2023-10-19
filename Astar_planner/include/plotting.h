#ifndef PLOTTING_H
#define PLOTTING_H

#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include "matplotlibcpp.h"
#include "common_datastruct.h"

namespace plt = matplotlibcpp;

class Plotting{

public:

    void plot_grid(std::vector<std::vector<int>> grid);

    void plot_start_dest(loc2d start, loc2d dest);

    void plot_path(std::stack<loc2d> path);

    void show_image(std::vector<std::vector<int>> grid);

};







#endif