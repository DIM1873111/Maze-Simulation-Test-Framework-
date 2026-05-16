#include "maze.h"
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <algorithm>  // shuffle
#include <random>
#include <functional>
#include <unordered_map>
#include <chrono>



Maze_AI::Maze_AI(){
mapMax = Xmax*Ymax;
Startrunning_Time_Timer();//时间初始化
/*如果需要初始化数据 放在这里*/
}

Maze_AI::~Maze_AI(){
    /*如果需要清理资源，在这里写*/
}
//提供添加算法仓库函数

void Maze_AI::add_Create(std::string Name,std::string Introduction,std::function<void()> Run,int Type){
Algorithm_Library_Create.push_back(
    {
        std::move(Name),//名字
        std::move(Introduction),//介绍
        std::move(Run),//函数地址
        Type//类型
    }//move提升性能
);
}

void Maze_AI::add_Search(std::string Name,std::string Introduction,std::function<void()> Run,int Type){
Algorithm_Library_Search.push_back(
    {
        std::move(Name),//名字
        std::move(Introduction),//介绍
        std::move(Run),//函数地址
        Type//类型
    }
);
}

//初始化函数实现

void Maze_AI::Startrunning_Time_Timer(){//时间初始化
auto Start_Time_void = std::chrono::duration_cast<std::chrono::seconds>(
std::chrono::system_clock::now().time_since_epoch()
);
Start_Time = Start_Time_void.count();
}

void Maze_AI::End_Point(){//终起xy位置初始化
Starting_point_Y = Starting_point/Xmax;
Starting_point_X = Starting_point-Starting_point_Y*Xmax;

Destination_Y = Destination/Xmax;
Destination_X = Destination-Destination_Y*Xmax;
}



//数据初始化以及算法选择函数实现
void Maze_AI::Parameter_Start_data(int Parameter,char* mapdata[]){//启动模式选择(参数模式启动)(数据初始化)
if(Parameter==5){//检查参数合理
int 
mapx = std::stoi(mapdata[1]),
mapy = std::stoi(mapdata[2]);
if(mapx<4 || mapy<4){exits("Parameter_error! [Name_-x_-y]",Error_exit);}//退出
Xmax = mapx,Ymax =mapy;

int 
sea = std::stoi(mapdata[3]),
cre = std::stoi(mapdata[4]);

maze_map.assign(Ymax, std::vector<int>(Xmax,Maze_walls_Enum));
Algorithm_selection(sea,cre);
}else if(Parameter >= 2){
std::cout << "The parameters you provided are insufficient. Startup method:" << std::endl;
std::cout << "Parameter Start: [Program Name(Location)] [mapx] [mapy] [Create Number] [Search Number]"  << std::endl;
std::cout << "Or enter after opening the program: [mapx] [mapy] [Create Number] [Search Number]" << std::endl;
std::cout << "Current program version:\033[4m" << Project_Version << "\033[0m" <<std::endl;
exits("Guide Ended",Exit_early);
}else{
maae_data_initialization();//正常模式启动  
    }
}

//显示菜单以及启动数据检查
void Maze_AI::maae_data_initialization(){//显示菜单(交互模式启动)
    int XnewValue=0,YnewValue=0,Pathfinding=0,Generation=0;
    std::cout << "[Dim|C++]Maze_Simulation_\033[4m" << Project_Version << "\033[0m" << std::endl;
    if(Algorithm_Library_Create.size()==0){
    std::cout << "\033[2mGeneration algorithm not found\033[0m" << std::endl;
    }else{
    for(int i = 0 ;i<Algorithm_Library_Create.size();i++){
        std::cout <<Algorithm_Library_Create[i].Name << "-Number|Type[" << i << "|" << Algorithm_Library_Create[i].Type << "]" << std::endl;
        std::cout << "\033[2m"<<Algorithm_Library_Create[i].Introduction << "\033[0m" << std::endl;
        }
    }
    std::cout << "\033[1m- - - - - - - - - -\033[0m"<< std::endl;
    if(Algorithm_Library_Search.size()==0){
    std::cout << "\033[2mSearch algorithm not found\033[0m" << std::endl;
    }else{
    for(int i = 0 ;i<Algorithm_Library_Search.size();i++){
        std::cout <<Algorithm_Library_Search[i].Name << "-Number|Type[" << i << "|" << Algorithm_Library_Search[i].Type << "]" << std::endl;
        std::cout << "\033[2m"<<Algorithm_Library_Search[i].Introduction << "\033[0m" << std::endl;
        }
    }
    std::cout << "> [mapX] [mapY] [Generation] [Pathfinding]"<<std::endl;
    std::cout << "Please_enter.-->";
    std::cin >> XnewValue >> YnewValue >> Generation >> Pathfinding;
    if(XnewValue>4&&YnewValue>4){
        Xmax=XnewValue;Ymax=YnewValue;
    }else{
        exits("The size you entered is too small X|Y",Error_exit);
        }
    //Cursor_position = Algorithm_Library_Create.size()*2+Algorithm_Library_Search.size()*2 + 2;
   maze_map.assign(Ymax, std::vector<int>(Xmax,Maze_walls_Enum));  //初始值0
   Algorithm_selection(Generation,Pathfinding);
}


//主要渲染函数
void Maze_AI::Map_loading(std::string mapdata){//迷宫渲染

auto Start_Time_Current = std::chrono::duration_cast<std::chrono::seconds>(
std::chrono::system_clock::now().time_since_epoch());
int Time_Current = Start_Time_Current.count();
Time_Duration = Time_Current - Start_Time;

std::cout << "\033[?25l";
std::cout << "\033[" << Ymax << "A";
for(int i = 0;i<Ymax;i++){
    std::cout << std::flush;
    for(int j = 0;j<Xmax;j++){
        Render_cout(i,j);
        }std::cout << std::endl;
    }
    std::cout<< "\33[2K\r" << "Current.\033[4m"<< mapdata << "\033[0m\033[2m[" <<Time_Duration<<"/S]"<<"\033[0m\033[1A\r";
    //Offset = true;
}

void Maze_AI::Render_cout(int y,int x){
switch(maze_map[y][x]){
    case Maze_empty_Enum:
    std::cout << ".";break;
    case Maze_walls_Enum:
    std::cout << "#";break;
    case Maze_Destination:
    std::cout << "\033[91m\033[4mE\033[0m";break;
    case Maze_Starting_point:
    std::cout << "\033[94m\033[4mS\033[0m";break;
    case Maze_aipath_Enum:
    std::cout << "~";break;
    case Maze_Shortest_route:
    std::cout << "\033[35m=\033[0m";break;
    case Maze_Searching_Traces1:
    std::cout << "+";break;
    case Maze_Searching_Traces2:
    std::cout << "-";break;
    case Gold_Coin_Special:
    std::cout << "\033[38;5;226m\033[2m$\033[0m";break;
    case Scan_Mark:
    std::cout << "\033[32m\033[2m.\033[0m";break;
    case Gold_Coin_Tag:
    std::cout << "\033[38;5;226m\033[4m\033[2m$\033[0m";break;
    default://未知数据
    std::cout << "\033[31m?\033[0m";break;
    }
std::cout << " ";//间隔    
}

void Maze_AI::clean_animation(){//清屏
    std::cout << "\n\r";
    for(int i = 0;i<Ymax+1;i++){
        std::cout << "\33[2K\r" << "\033[1A";
    }
}



void Maze_AI::Algorithm_selection(int Create,int Search){//算法启动以及数据检查
//检查数据合理性    
if(Ymax<4 || Xmax<4){
exits("XY map data error [XY<4]",Error_exit);
}
if(Create>=Algorithm_Library_Create.size() || Create<0){
    exits("Cannot find the create algorithm library",Error_exit);
}
if(Search>=Algorithm_Library_Search.size() || Search<0){
    exits("Cannot find the search algorithm library",Error_exit);
}
if(Algorithm_Library_Create[Create].Type!=Algorithm_Library_Search[Search].Type){
    exits("The type of the two algorithms does not match",Error_exit);
}
//int Cursor = Algorithm_Library_Search.size()*2+Algorithm_Library_Create.size()*2 + 2;
//std::cout << "\033[" << Cursor << "A";

//记录选择的算法(结算数据时使用)
Select_Create = Create;
Select_Search = Search;

    clean_animation();//清理屏幕
    Algorithm_Library_Create[Create].Run();//生成
    End_Point();//终起xy位置初始化
    Algorithm_Library_Search[Search].Run();//搜索
    map_Render_Display();//最后一次刷新地图
    Settlement();//结算数据

}

//工具函数实现
bool Maze_AI::Boundary_check(int x,int y){//边界检查
    return (x < Xmax && x >= 0 && y < Ymax && y >= 0);
}

bool Maze_AI::Check_index(int Index){
    return (Index >= 0) && (Index + 1 <= Xmax * Ymax);
}

void Maze_AI::Mark_Trace(int x,int y,int Type){//快捷标记
    if(Boundary_check(x,y)){
    maze_map[y][x]=Type;
    }else{
    exits("Exceeding map boundaries when setting a marker",Error_exit);
    }

}
int Maze_AI::Location_Marker(int x,int y){//返回位置标记
    return maze_map[y][x];
}
bool Maze_AI::Tag_Information(int x,int y,int Type){//检查特定标记
    if(!Boundary_check(x,y)){
    exits("Exceeds map range when checking the mark",Error_exit);
    }
    return (maze_map[y][x]==Type);
}
int Maze_AI::X_Map_Max(){
return Xmax;
}
int Maze_AI::Y_Map_Max(){
return Ymax;
}
void Maze_AI::Set_destination(int Index){
if(Check_index(Index)){
Destination=Index;    
    }else{
exits("Exceeding the map boundaries when setting the destination",Error_exit);
    }
}
void Maze_AI::Set_Starting_point(int Index){
if(Check_index(Index)){
Starting_point=Index;    
    }else{
exits("Exceeding the map boundaries when setting the starting point",Error_exit);
    }
}
int Maze_AI::Obtain_destination(){
    return Destination;
}
int Maze_AI::Obtain_Starting_point(){
    return Starting_point;
}
bool Maze_AI::Check_Destination(int x,int y){
return (Destination_X==x && Destination_Y==y);
}
bool Maze_AI::Check_starting_point(int x,int y){
return (Starting_point_X==x && Starting_point_Y==y);
}
int Maze_AI::Get_indexX(int Index){
if(Check_index(Index)){
    return Index%Xmax;
}else{
    exits("Exceeding the map boundaries when obtaining the indexX",Error_exit);
    return 0;
}
}
int Maze_AI::Get_indexY(int Index){
if(Check_index(Index)){
    return Index/Xmax;
}else{
    exits("Exceeding the map boundaries when obtaining the indexY",Error_exit);
    return 0;
}
}
//End_Point

//收尾函数实现
void Maze_AI::map_Render_Display(){//刷新起点终点x'x
Display_Destination_Starting_point();//刷新起/终点
Map_loading("Completed!");//最后刷新
}
void Maze_AI::Display_Destination_Starting_point(){//刷新起点终点
int YStarting_point = Starting_point/Xmax;//加载起点(Y)
int YDestination = Destination/Xmax;//加载终点(Y)
maze_map[YStarting_point][Starting_point-(YStarting_point*Xmax)]=Maze_Starting_point;
maze_map[YDestination][Destination-(YDestination*Xmax)]=Maze_Destination;
}
void Maze_AI::Settlement(){//结算函数
std::cout << "\n\r\33[2K";
std::cout <<"Method.[\033[4m"<< Algorithm_Library_Create[Select_Create].Name << "\033[0m]and[\033[4m" << Algorithm_Library_Search[Select_Search].Name << "\033[0m]" << std::endl;
std::cout << "Time["<<Time_Duration<<"/s]["<< Xmax << "*" << Ymax << "]\033[94m[Start|" << Starting_point_X << "," << Starting_point_Y << "]\033[35m=>\033[91m[End|" << Destination_X << "," << Destination_Y << "]\033[0m" << std::endl;
}
void Maze_AI::exits(std::string Error_message,int Exit_Type){//错误处理
switch (Exit_Type)
{
case Error_exit://检查退出类型
std::cout << "\033[31m[!]\033[0mError message.-->\033[4m"<< Error_message << "\033[0m";
break;

case Exit_early:
std::cout << "\033[2m[i]\033[0mProgram exit.-->\033[4m"<< Error_message << "\033[0m";
break;

default:
    break;
}
exit(0);//程序退出
}