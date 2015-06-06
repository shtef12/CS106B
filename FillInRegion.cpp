#include <iostream>
#include "console.h"
#include "simpio.h"
#include "grid.h"
using namespace std;

void fillRegion(Grid<bool> &grid, int row, int col)
{
    if(grid.get(row,col) == true)
    {
        return;
    }else{
        grid.set(row,col,true);
        if(grid.inBounds(row,col+1))
            fillRegion(grid,row,col+1);
        if(grid.inBounds(row,col-1))
            fillRegion(grid,row,col-1);

        if(grid.inBounds(row+1,col))
            fillRegion(grid,row+1,col);
        if(grid.inBounds(row-1,col))
            fillRegion(grid,row-1,col);
    }
}

int main()
{
    Grid<bool> newGrid(5,5);
    newGrid.set(0,2,true);
    newGrid.set(1,1,true);
    newGrid.set(2,0,true);
    newGrid.set(4,3,true);
    newGrid.set(3,4,true);
    fillRegion(newGrid,2,2);
    cout << newGrid.toString2D() << endl;
    return 0;
}
