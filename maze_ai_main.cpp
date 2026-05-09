#include "Maze_ai_lx.h"   // 1. 包含自定义类的头文件
#include <iostream>

#include <vector>
#include <queue>
#include <string>

int main(int argc, char* argv[]){
Maze_AI Maze_Simulation;
int i = 1;
// while(i){
Maze_Simulation.Parameter_Start_data(argc,argv);//模式选择
//Maze_Simulation.map_Render_Display();
//Maze_Simulation.clean_animation();
Maze_Simulation.Settlement();
return 0;
//    }
}