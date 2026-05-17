#include "maze.h"//框架头文件
#include "Algorithm_Repository_v1.h"//算法仓库头文件
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <algorithm>  // shuffle
#include <random>
#include <functional>
#include <unordered_map>
#include <sstream>
#include <cstdlib>

void Maze_Factory(Maze_AI& maze){//生成迷宫(DFS(随机))
static 
int //偏移量|默认方向
Guide[4]={0,1,2,3},//偏移量编号
XOffset[4]={-2,0,0,2},//x偏移量
YOffset[4]={0,-2,2,0};//y偏移量
std::vector<int> Search_data;
std::random_device rd;  // 获取真随机数种子
std::mt19937 rng(rd());   // 创建随机数引擎
std::uniform_int_distribution<int> dist(0,(maze.X_Map_Max()*maze.Y_Map_Max()));
maze.Set_Starting_point(dist(rng));
Search_data.push_back(maze.Obtain_Starting_point());
while (Search_data.size()){
std::shuffle(Guide, Guide + 4, rng);
int last_item = Search_data.size()-1;
int seaY = Search_data[last_item]/maze.X_Map_Max();
int seaX = Search_data[last_item]-seaY*maze.X_Map_Max();
int Dead_end = true;//如果没路 那么删除(换一个)true/false
int seaxs,seays;
for(int i=0;i<4;i++){
    seaxs = seaX,seays = seaY;
    seaxs = seaxs + (XOffset[Guide[i]]);
    seays = seays + (YOffset[Guide[i]]);
    if(maze.Boundary_check(seaxs,seays) && !maze.Location_Marker(seaxs,seays)){
    maze.Mark_Trace(seaxs-(XOffset[Guide[i]]/2),seays-(YOffset[Guide[i]]/2),1);
    maze.Mark_Trace(seaxs,seays,1);
    std::stringstream Current_content;//刷新屏幕
    Current_content << "Create a maze-->[" << seaxs << "," << seays << "," << Search_data.size() <<"]";
    maze.Map_loading(Current_content.str());
    Search_data.push_back(seays*maze.X_Map_Max()+seaxs);
    maze.Set_destination(seays*maze.X_Map_Max()+seaxs);
    Dead_end=false;
    break;
    }
}

if(Dead_end){Search_data.erase(Search_data.begin()+last_item);}
    }
int YStarting_point = maze.Obtain_Starting_point()/maze.X_Map_Max();//加载起点(Y)
int YDestination = maze.Obtain_destination()/maze.X_Map_Max();//加载终点(Y)
maze.Mark_Trace(maze.Obtain_Starting_point()-(YStarting_point*maze.X_Map_Max()),YStarting_point,maze.Maze_Starting_point);
maze.Mark_Trace(maze.Obtain_destination()-(YDestination*maze.X_Map_Max()),YDestination,maze.Maze_Destination);
}

