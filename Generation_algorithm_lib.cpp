#include "Maze_ai_lx.h"
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


void Maze_AI::Maze_Factory(){//生成迷宫(DFS(随机))
static 
int //偏移量|默认方向
Guide[4]={0,1,2,3},//偏移量编号
XOffset[4]={-2,0,0,2},//x偏移量
YOffset[4]={0,-2,2,0};//y偏移量
std::random_device rd;  // 获取真随机数种子
std::mt19937 rng(rd());   // 创建随机数引擎
std::uniform_int_distribution<int> dist(0,(Xmax*Ymax));
Starting_point = dist(rng);
Search_data.push_back(Starting_point);
while (Search_data.size()){
std::shuffle(Guide, Guide + 4, rng);
int last_item = Search_data.size()-1;
int seaY = Search_data[last_item]/Xmax;
int seaX = Search_data[last_item]-seaY*Xmax;
int Dead_end = 1;//如果没路 那么删除(换一个)
for(int i=0;i<4;i++){
    int seaxs = seaX,seays = seaY;
    seaxs = seaxs + (XOffset[Guide[i]]);
    seays = seays + (YOffset[Guide[i]]);
    if(seays<Ymax && seays>=0 && seaxs<Xmax && seaxs>=0 && !maze_map[seays][seaxs]){
    maze_map[seays-(YOffset[Guide[i]]/2)][seaxs-(XOffset[Guide[i]]/2)]=1;
    maze_map[seays][seaxs]=1;
    std::stringstream Current_content;//刷新屏幕
    Current_content << "Create a maze-->[" << seaxs << "," << seays << "," << Search_data.size() <<"]";
    Map_loading(Current_content.str());
    Search_data.push_back(seays*Xmax+seaxs);Destination=seays*Xmax+seaxs;
    Dead_end=0;
    break;
    }
}
if(Dead_end){Search_data.erase(Search_data.begin()+last_item);}
    }
int YStarting_point = Starting_point/Xmax;//加载起点(Y)
int YDestination = Destination/Xmax;//加载终点(Y)
maze_map[YStarting_point][Starting_point-(YStarting_point*Xmax)]=Maze_Starting_point;
maze_map[YDestination][Destination-(YDestination*Xmax)]=Maze_Destination;
}
void Maze_AI::Maze_Factory_Terrain(){//生成迷宫(DFS(随机))地形版
static 
int //偏移量|默认方向
Guide[4]={0,1,2,3},//偏移量编号
XOffset[4]={-2,0,0,2},//x偏移量
YOffset[4]={0,-2,2,0};//y偏移量
std::random_device rd;  // 获取真随机数种子
std::mt19937 rng(rd());   // 创建随机数引擎
std::uniform_int_distribution<int> dist(0,(Xmax*Ymax));
Starting_point = dist(rng);
Search_data.push_back(Starting_point);
while (Search_data.size()){
std::shuffle(Guide, Guide + 4, rng);
int last_item = Search_data.size()-1;
int seaY = Search_data[last_item]/Xmax;
int seaX = Search_data[last_item]-seaY*Xmax;
int Dead_end = 1;//如果没路 那么删除(换一个)
for(int i=0;i<4;i++){
    int seaxs = seaX,seays = seaY;
    seaxs = seaxs + (XOffset[Guide[i]]);
    seays = seays + (YOffset[Guide[i]]);
    if(seays<Ymax && seays>=0 && seaxs<Xmax && seaxs>=0 && !maze_map[seays][seaxs]){
    maze_map[seays-(YOffset[Guide[i]]/2)][seaxs-(XOffset[Guide[i]]/2)]=1;
    maze_map[seays][seaxs]=Maze_empty_Enum;
    std::stringstream Current_content;//刷新屏幕
    Current_content << "Create a maze-->[" << seaxs << "," << seays << "," << Search_data.size() <<"]";
    Map_loading(Current_content.str());
    Search_data.push_back(seays*Xmax+seaxs);Destination=seays*Xmax+seaxs;
    Dead_end=0;
    break;
    }
}
if(Dead_end){Search_data.erase(Search_data.begin()+last_item);}
    }

std::uniform_int_distribution<int> dist_1(0,3);

for(int i = 0;i<Ymax;i++){//生成金币
    for(int j = 0;j<Xmax;j++){
        if(maze_map[i][j]==Maze_empty_Enum){
            int Random_coins = dist_1(rng);
            if(Random_coins==0){
                maze_map[i][j]=Gold_Coin_Special;
                std::stringstream Current_content;//刷新屏幕
                Current_content << "Generating coins at [" << i << "," << j << "] position...";
                Map_loading(Current_content.str());
            }
        }
    }
}


//int Random_coins = dist(rng);

int YStarting_point = Starting_point/Xmax;//加载起点(Y)
int YDestination = Destination/Xmax;//加载终点(Y)
maze_map[YStarting_point][Starting_point-(YStarting_point*Xmax)]=Maze_Starting_point;
maze_map[YDestination][Destination-(YDestination*Xmax)]=Maze_Destination;
}