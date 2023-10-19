## An A* Algorithm

### Algorithm
- Using Eclidean Heristic to find the shortest path

- cost (Accumulated moving distance + euclidean heristic): <br>
(1) successor.g: q.g + qp.g (distance between the q to successor p) <br>
(2) successor.h: heristic distance from successor p to the destination <br>
(3) successor.c = successor.g + successor.h

- Algorithm Pseudo code:
```
1.initialize open list (priority queue (minheap))
             closed list (2d boolean vector)
             predecessors (2d vector of cell)
    Put the start point to the open list, call it q
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
```

### Data structure
- Open list: priority queue (minheap)
```
struct pPair{
    double cost; 
    int i,j;  // location
};
struct Comparecost { // use for ordering queue by the value of cost in ascending order
    bool operator()(pPair const& p1, pPair const& p2){
        return p1.cost > p2.cost;
    }
};

std::priority_queue< pPair, std::vector<pPair>, Comparecost > openList;
```

- Cloased list: 2d boolean vector
```
std::vector<std::vector<bool>> closedList(row, std::vector<bool>(col, false));
```

- predecessors: 2d vector of cell (linked tree)
```
struct cell {
    // Row and Column index of its parent
    int parent_i, parent_j;
    // c = g + h
    double c, g, h;
};

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
```


### Test
![result](Result_fig.png) 

