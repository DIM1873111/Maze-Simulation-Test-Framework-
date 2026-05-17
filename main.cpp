#include "maze.h"   // 1. 包含自定义类的头文件
#include "Algorithm_Repository_v1.h"//算法仓库头文件
#include <iostream>
#include <vector>
#include <queue>
#include <string>

int main(int argc, char* argv[]){
Maze_AI Maze_Simulation;
Maze_Simulation.add_Create(
    "DFS[0.0.7]",
    "adaptation framework[v1]",
    [&Maze_Simulation]() { Maze_Factory(Maze_Simulation); },
    100 
);
Maze_Simulation.add_Search(
    "BFS[0.0.9]",
    "adaptation framework[v1.1]",
    [&Maze_Simulation]() { Maze_pathfinding(Maze_Simulation); },
    100
);
Maze_Simulation.add_Search(
    "Bidirectional_BFS[0.0.7]",
    "adaptation framework[v1.2]",
    [&Maze_Simulation]() { Bidirectional_BFS(Maze_Simulation); },
    100
);
Maze_Simulation.add_Search(
    "Algorithm_A[0.0.7]",
    "adaptation framework[v1]",
    [&Maze_Simulation]() { Search_Algorithm_Algorithm_A(Maze_Simulation); },
    100
);
Maze_Simulation.add_Search(
    "Pseudo-dynamic programming[0.0.71]",
    "Composite algorithm",
    [&Maze_Simulation]() { Maze_Path_DP(Maze_Simulation); },
    100
);
Maze_Simulation.add_Search(
    "Dynamic Programming[0.0.72]",
    "Dynamic Programming[v1]",
    [&Maze_Simulation]() { Maze_Path_DP_True(Maze_Simulation);},
    100
);
Maze_Simulation.add_Search(
    "Bidirectional_A_Algorithm",
    "Bidirectional_A_Algorithm[Av2]",
    [&Maze_Simulation]() { Bidirectional_A_Algorithm(Maze_Simulation); },
    100
);
Maze_Simulation.add_Search(
    "Algorithm_A_Revision",
    "Algorithm_A_Revision[Av1]",    
    [&Maze_Simulation]() { Algorithm_A_Revision(Maze_Simulation); },
   100
);
Maze_Simulation.add_Search(
    "Search_Dijkstra_true",
    "Search_Dijkstra",
    [&Maze_Simulation]() { Search_Dijkstra_true(Maze_Simulation); },
    100
    );
Maze_Simulation.add_Search(
    "JPS",
    "Search_Algorithm_JPS[Av2]",
    [&Maze_Simulation]() { Search_Algorithm_JPS(Maze_Simulation); },
   100
);
Maze_Simulation.Parameter_Start_data(argc,argv);//模式选择(主函数)

return 0;

}