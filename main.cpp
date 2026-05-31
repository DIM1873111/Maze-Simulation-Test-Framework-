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
    100,
    Maze_Simulation.Maze_walls_Enum
);
Maze_Simulation.add_Create(
    "Prim[0.0.8]",
    "adaptation framework[v1.1]",
    [&Maze_Simulation]() { generateMazeByPrim(Maze_Simulation); },
    100,
    Maze_Simulation.Maze_walls_Enum
);
Maze_Simulation.add_Create(
    "Recursive_Division[0.0.7]",
    "adaptation framework[v1.1]",
    [&Maze_Simulation]() { Recursive_Division(Maze_Simulation); },
    100,
    Maze_Simulation.Maze_empty_Enum
);
//void MST_Minimum_Spanning_Tree(Maze_AI& maze)
Maze_Simulation.add_Create(
    "MST_Minimum_Spanning_Tree[0.0.7]",
    "adaptation framework[v1.1]",
    [&Maze_Simulation]() { MST_Minimum_Spanning_Tree(Maze_Simulation); },
    100,
    Maze_Simulation.Maze_walls_Enum
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
//Iterative_Deepening
Maze_Simulation.add_Search(
    "Iterative_Deepening",
    "Iterative_Deepening[Av1]",
    [&Maze_Simulation]() { Iterative_Deepening(Maze_Simulation); },
    100
);
//Iterative_Deepening_A_Star
Maze_Simulation.add_Search(
    "Iterative_Deepening_A_Star",
    "Iterative_Deepening_A_Star[Av1]",
    [&Maze_Simulation]() { Iterative_Deepening_A_Star(Maze_Simulation); },
    100
);
//Search_Algorithm_IDA_Star
Maze_Simulation.add_Search(
    "Search_Algorithm_IDA_Star",
    "Search_Algorithm_IDA_Star[Av1]",
    [&Maze_Simulation]() { Search_Algorithm_IDA_Star(Maze_Simulation); },
    100
);
//randomized_DepthFirst_Search
Maze_Simulation.add_Search(
    "randomized_DepthFirst_Search",
    "randomized_DepthFirst_Search[v1.2]",
    [&Maze_Simulation]() { randomized_DepthFirst_Search(Maze_Simulation); },
    100
);
//lrta_star_search
Maze_Simulation.add_Search(
    "lrta_star_search",
    "“Dot” search series[v1]",
    [&Maze_Simulation]() { lrta_star_search(Maze_Simulation); },
    100
);
//Greedy_A_search
Maze_Simulation.add_Search(
    "Greedy_ADIM_search",
    "“Dot” search series[v1],Original algorithm!",
    [&Maze_Simulation]() { Greedy_ADIM_search(Maze_Simulation); },
    100
);
//Ant_Colony_Algorithm
Maze_Simulation.add_Search(
    "Ant_Colony_Algorithm",
    "Ant Colony Optimization[v1]",
    [&Maze_Simulation]() { Ant_Colony_Algorithm(Maze_Simulation); },
    100
);
//group_ADIM__search
Maze_Simulation.add_Search(
    "group_ADIM__search",
    "Original algorithm!",
    [&Maze_Simulation]() { group_adim__search(Maze_Simulation); },
    100
);
//Two_way_group_adim__search
Maze_Simulation.add_Search(
    "Two_way_group_adim__search",
    "Original algorithm!",
    [&Maze_Simulation]() { Two_way_group_adim__search(Maze_Simulation); },
    100
);
Maze_Simulation.Parameter_Start_data(argc,argv);//模式选择(主函数)

return 0;

}