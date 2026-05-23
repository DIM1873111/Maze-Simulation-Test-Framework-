#ifndef MYCLASS_H
#define MYCLASS_H

#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <unordered_map>

#include <SFML/Graphics.hpp>

class Maze_AI
{
private://私有
int Xmax = 0,Ymax = 0;
int Destination=0;//终点数据
int Starting_point=0;//起点数据

int Destination_X;//终点X
int Destination_Y;//终点Y
int Starting_point_X;//起点X
int Starting_point_Y;//起点Y

int map_Smallest = 5;//最小地图尺寸
int Map_Maximum = 300;//最大地图尺寸

float Proportion = 20;//缩放比例

int Rendering_speed = 50;//渲染速度
int Minimum_speed = 0;//最快渲染速度
int Maximum_speed = 300;//最慢渲染速度
int Speed_Adjustment = 10;//渲染速度调节(大小)
sf::RenderWindow window;//创建窗口

bool Pause_Status = false;//暂停状态
bool ESC_Exit_Status = false;//ESC退出状态
bool Accelerate_Status = false;//加速状态
bool Decelerate_Status = false;//减速状态


int mapMax;//地图总大小
int Start_Time;//运行时间计时器

int Time_Duration;//运行时间

int Select_Create;//用户选择的生成算法
int Select_Search;//用户选择的搜索算法
std::string Project_Version = "V3.3.16(Graphical)";//项目版本


std::vector<std::vector<int>> maze_map;//地图数据


struct Algorithm_structure_Create{
std::string Name;//算法名称
std::string Introduction;//算法简介
std::function<void()> Run;//函数指针
int Type;//算法类型
int Initialization;//初始化标记
};
struct Algorithm_structure_Search{
std::string Name;//算法名称
std::string Introduction;//算法简介
std::function<void()> Run;//函数指针
int Type;//算法类型
};

struct CircleData {//圆形数据结构

    float size;  // 大小
    sf::Color color;   // 颜色
};
     //std::string type;  // "circle", "rectangle", "triangle"
std::vector<std::vector<CircleData>> Rendering_map;//渲染数据缓存

std::vector<Algorithm_structure_Create> Algorithm_Library_Create;//算法库
std::vector<Algorithm_structure_Search> Algorithm_Library_Search;//算法库



enum Exit_Status{//程序退出类型
Error_exit = false,//错误退出
Exit_early = true,//正常提前退出
};

enum Log_Type{
Log_Error = 0,//错误日志
Log_Warning = 1,//警告日志(配项调整以及警告等等)
Log_Information = 2,//信息日志
};


//主函数
void maae_data_initialization();

void Display_Destination_Starting_point();

void Processing_window();//处理窗口渲染
void Interactive_State_window();////处理交互


void Render_cout(int y,int x);//初始化渲染内容


//初始化函数
void End_Point();//终起xy初始化(运行完生成算法后)
void Startrunning_Time_Timer();//时间初始化
//std::string GetFormattedTime();//时间格式化[hh:mm:ss]
void exits(std::string Error_message,int Exit_Type = Error_exit);//退出函数

void map_Render_Display();//最后一次刷新起点终点

void Settlement();//结算数据

void Log_output(std::string message,Log_Type Type);
public://公有
std::stringstream Log_stream;//日志流
enum Mazemapdata{//地图数据枚举
    //标记类
Maze_Destination=-1,//终点
Maze_Starting_point=-2,//起点
Empty_data_Enum=-3,
Gold_Coin_Special=-4,//金币特殊标记
Gold_Coin_Tag=-5,//扫描金币标记
Scan_Mark=-6,//扫描标记
    //实体类
Maze_walls_Enum=0,//墙壁    
Maze_empty_Enum=1,//空
Maze_aipath_Enum=2,//寻找痕迹
Maze_Searching_Traces1=3,//寻找痕迹1
Maze_Searching_Traces2=4,//寻找痕迹2
Maze_Shortest_route=5,//最短路线标记

};
//工具函数
bool Boundary_check(int x,int y);//检查边界是否合法
bool Check_index(int Index);//检查索引是否合法
bool Check_Destination(int x,int y);//检查当前位置是否终点
bool Check_starting_point(int x,int y );//检查当前位置是否起点
bool Tag_Information(int x,int y,int Type);//检查特定的标记是否符合要求

void Mark_Trace(int x,int y,int Type);//设置地图数据标记(x位置，y位置，标记的类型)
int Location_Marker(int x,int y);//返回位置地图数据标记(x位置，y位置)

int X_Map_Max();//返回地图Xmax
int Y_Map_Max();//返回地图Ymax

void Set_destination(int Index);//设置地图终点
void Set_Starting_point(int Index);//设置地图起点

int Obtain_destination();//获取地图终点位置(引索)
int Obtain_Starting_point();//获取地图起点位置(引索)
int Random_number(int Min,int Max);//随机数生成


int Get_indexX(int Index);//计算引索对应的X坐标
int Get_indexY(int Index);//计算引索对应的Y坐标
void add_Create(std::string Name,std::string Introduction,std::function<void()> Run,int Type,Mazemapdata Initialization);//添加创造库
void add_Search(std::string Name,std::string Introduction,std::function<void()> Run,int Type);//添加搜索库
void Map_loading(std::string mapdata);//刷新地图

//主函数

void Parameter_Start_data(int Parameter,char* mapdata[]);//启动模式选择

void Algorithm_selection(int Generate,int Search);//主函数启动

    Maze_AI(/* args */);
    ~Maze_AI();
};

#endif