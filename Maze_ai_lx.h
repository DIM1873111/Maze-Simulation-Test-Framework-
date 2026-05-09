#ifndef MYCLASS_H
#define MYCLASS_H

#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <unordered_map>

class Maze_AI
{
private://私有
int Xmax = 0,Ymax = 0;
int Destination;//终点数据
int Starting_point;//起点数据
int mapMax;
int Start_Time;

int Time_Duration;//运行时间

int Select_Create;//用户选择的生成算法
int Select_Search;//用户选择的搜索算法

//int Cursor_position = 0;//光标位置(暂时未使用)

std::vector<int> Search_data;
std::vector<int> historymap_data;
std::vector<std::vector<int>> maze_map;

struct Algorithm_structure_Create{
std::string Name;//算法名称
std::string Introduction;//算法简介
std::function<void()> Run;//函数指针
int Type;//算法类型
};
struct Algorithm_structure_Search{
std::string Name;//算法名称
std::string Introduction;//算法简介
std::function<void()> Run;//函数指针
int Type;//算法类型
};

std::vector<Algorithm_structure_Create> Algorithm_Library_Create;//算法库
std::vector<Algorithm_structure_Search> Algorithm_Library_Search;//算法库



enum Mazemapdata{//地图数据枚举
Maze_Destination=-1,//终点
Maze_Starting_point=-2,//起点
Empty_data_Enum=-3,
Maze_walls_Enum=0,//墙壁    
Maze_empty_Enum=1,//空
Maze_aipath_Enum=2,//寻找痕迹
Maze_Searching_Traces1=3,//寻找痕迹1
Maze_Searching_Traces2=4,//寻找痕迹2
Maze_Shortest_route=5,//最短路线标记

Gold_Coin_Special=-4,//金币特殊标记
Gold_Coin_Tag=-5,//扫描金币标记
Scan_Mark=-6//扫描标记

};

enum Algorithm_Enum{//算法枚举
Common_Enum=100,//正常类型
Special_Terrain_Enum=101,//特殊类型(地形)
//扩展
};
void maae_data_initialization();

void Display_Destination_Starting_point();

void Map_loading(std::string mapdata);
void Render_cout(int y,int x);

//int Get_cursor_position();
//初始化函数

void Load_algorithm_library();//初始化列表
void Startrunning_Time_Timer();//时间初始化

//算法函数

void Maze_Factory();//生成迷宫DFS
void Maze_Factory_Terrain();//生成迷宫DFS地形版
void Maze_pathfinding();//寻找路线BFS
void Maze_Path_2BFS();//寻找路线双向BFS
void Search_Algorithm_Algorithm_A();//寻找路线A*算法
void Maze_Path_DP();//寻找路线动态规划(伪)
void Maze_Path_DP_Terrain();//寻找路线动态规划地形版(伪)
void Maze_Path_DP_True();//寻找路线动态规划(真)
void Search_Algorithm_Algorithm_A_Two_way();//寻找路线A*算法双向


void exits(std::string Error_message);//退出函数

void map_Render_Display();//最后一次刷新起点终点

public://公有

void clean_animation();//清屏

void Settlement();//结算数据

void Parameter_Start_data(int Parameter,char* mapdata[]);//启动模式选择

void Algorithm_selection(int Generate,int Search);







    Maze_AI(/* args */);
    ~Maze_AI();
};


//Maze_AI::Maze_AI(/* args */){}
//Maze_AI::~Maze_AI(){}


#endif