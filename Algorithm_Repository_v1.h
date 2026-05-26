#ifndef MAZE_FACTORY_H
#define MAZE_FACTORY_H

#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <unordered_map>

class Maze_AI;

void Maze_Factory(Maze_AI& maze);
void generateMazeByPrim(Maze_AI& maze);
void Recursive_Division(Maze_AI& maze);
void MST_Minimum_Spanning_Tree(Maze_AI& maze);

void Maze_pathfinding(Maze_AI& maze);
void Bidirectional_BFS(Maze_AI& maze);
void Search_Algorithm_Algorithm_A(Maze_AI& maze);
void Maze_Path_DP(Maze_AI& maze);
void Maze_Path_DP_True(Maze_AI& maze);
void Bidirectional_A_Algorithm(Maze_AI& maze);
void Algorithm_A_Revision(Maze_AI& maze);
void Search_Dijkstra_true(Maze_AI& maze);
void Search_Algorithm_JPS(Maze_AI& maze);
void Iterative_Deepening(Maze_AI& maze);
void Iterative_Deepening_A_Star(Maze_AI& maze);
void Search_Algorithm_IDA_Star(Maze_AI& maze);
#endif // MAZE_FACTORY_H