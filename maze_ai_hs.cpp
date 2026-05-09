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
#include <chrono>



Maze_AI::Maze_AI(){
/*
Xmax = 10;
Ymax = 10;
*/
mapMax = Xmax*Ymax;
Load_algorithm_library();//加载算法库
Startrunning_Time_Timer();//时间初始化
/*如果需要初始化数据 放在这里*/
}

Maze_AI::~Maze_AI(){
    /*如果需要清理资源，在这里写*/
}
//初始化函数实现
void Maze_AI::Load_algorithm_library(){//初始化
Algorithm_Library_Create.push_back(
    {
        "DFS_1",
        "Blind Digging Maze_DFS(V1.1)",
        std::bind(&Maze_AI::Maze_Factory, this),
        Common_Enum
    }
);
Algorithm_Library_Create.push_back(
    {
        "DFS_Terrain",
        "Blind Digging Maze_DFS_Terrain(V1.2)",
        std::bind(&Maze_AI::Maze_Factory_Terrain, this),
        Special_Terrain_Enum
    }
);

Algorithm_Library_Search.push_back(
    {
        "DFS1",
        "Flood Fill_DFS(1.1)",
        std::bind(&Maze_AI::Maze_pathfinding, this),
        Common_Enum
    }
);
Algorithm_Library_Search.push_back(
    {
        "DFS2",
        "Bigger Flood Fill_DFS2(V1.2)",
        std::bind(&Maze_AI::Maze_Path_2BFS, this),
        Common_Enum
    }
);
Algorithm_Library_Search.push_back(
    {
        "Algorithm_A",
        "A* Algorithm(V1.3)",
        std::bind(&Maze_AI::Search_Algorithm_Algorithm_A, this),
        Common_Enum
    }
);
Algorithm_Library_Search.push_back(
    {
        "DP",
        "Dynamic Programming(V1.4)",
        std::bind(&Maze_AI::Maze_Path_DP, this),
        Common_Enum
    }
);
Algorithm_Library_Search.push_back(
    {
        "DP_Terrain",
        "Dynamic Programming_Terrain(V1.4)",
        std::bind(&Maze_AI::Maze_Path_DP_Terrain, this),
        Special_Terrain_Enum
    }
);
Algorithm_Library_Search.push_back(
    {
        "DP_True",
        "Dynamic Programming_True(V1.5)",
        std::bind(&Maze_AI::Maze_Path_DP_True, this),
        Common_Enum
    }
);
Algorithm_Library_Search.push_back(
    {
        "2*Algorithm",
        "A* Algorithm(V1.7)",
        std::bind(&Maze_AI::Search_Algorithm_Algorithm_A_Two_way, this),
        Common_Enum
    }
);

}

void Maze_AI::Startrunning_Time_Timer(){//时间初始化
auto Start_Time_void = std::chrono::duration_cast<std::chrono::seconds>(
std::chrono::system_clock::now().time_since_epoch()
);
Start_Time = Start_Time_void.count();
}


//数据初始化以及算法选择函数实现
void Maze_AI::Parameter_Start_data(int Parameter,char* mapdata[]){//启动模式选择(参数模式启动)(数据初始化)
if(Parameter==5){//检查参数合理
int 
mapx = std::stoi(mapdata[1]),
mapy = std::stoi(mapdata[2]);
if(mapx<4 || mapy<4){exits("Parameter_error! [Name_-x_-y]");}//退出
Xmax = mapx,Ymax =mapy;

int 
sea = std::stoi(mapdata[3]),
cre = std::stoi(mapdata[4]);

maze_map.assign(Ymax, std::vector<int>(Xmax,Maze_walls_Enum));
Algorithm_selection(sea,cre);
}else if(Parameter>=2){
exits("Parameter_error! [Name_-x_-y] or [Name_-sea_-cre]");//退出
}else{
maae_data_initialization();//正常模式启动  
    }
}

//显示菜单以及启动数据检查
void Maze_AI::maae_data_initialization(){//显示菜单(交互模式启动)
    int XnewValue=0,YnewValue=0,Pathfinding=0,Generation=0;
    std::cout << "[Dim|C++]Maze_Simulation_v2.7" << std::endl;
    
    for(int i = 0 ;i<Algorithm_Library_Create.size();i++){
        std::cout <<Algorithm_Library_Create[i].Name << "-Number|Type[" << i << "|" << Algorithm_Library_Create[i].Type << "]" << std::endl;
        std::cout << "\033[2m"<<Algorithm_Library_Create[i].Introduction << "\033[0m" << std::endl;
    }    
    std::cout << "\033[1m- - - - - - - - - -\033[0m"<< std::endl;

    for(int i = 0 ;i<Algorithm_Library_Search.size();i++){
        std::cout <<Algorithm_Library_Search[i].Name << "-Number|Type[" << i << "|" << Algorithm_Library_Search[i].Type << "]" << std::endl;
        std::cout << "\033[2m"<<Algorithm_Library_Search[i].Introduction << "\033[0m" << std::endl;
    }
    std::cout << ">_[mapX]_[mapY]_[Generation_Pathfinding]"<<std::endl;
    std::cout << "Please_enter.-->";
    std::cin >> XnewValue >> YnewValue >> Generation >> Pathfinding;
    if(XnewValue>4&&YnewValue>4){
        Xmax=XnewValue;Ymax=YnewValue;
    }else{
        exits("The size you entered is too small X|Y");
        }
    //Cursor_position = Algorithm_Library_Create.size()*2+Algorithm_Library_Search.size()*2 + 2;
   maze_map.assign(Ymax, std::vector<int>(Xmax,Maze_walls_Enum));  //初始值0
   Algorithm_selection(Generation,Pathfinding);
}


//主要渲染函数
void Maze_AI::Map_loading(std::string mapdata){//迷宫渲染
//static bool Offset = false;
//if(Offset){std::cout << "\033[1A\r";}

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
exits("XY map data error [XY<4]");
}
if(Create>=Algorithm_Library_Create.size() || Create<0){
    exits("Cannot find the create algorithm library");
}
if(Search>=Algorithm_Library_Search.size() || Search<0){
    exits("Cannot find the search algorithm library");
}
if(Algorithm_Library_Create[Create].Type!=Algorithm_Library_Search[Search].Type){
    exits("The type of the two algorithms does not match");
}
//int Cursor = Algorithm_Library_Search.size()*2+Algorithm_Library_Create.size()*2 + 2;
//std::cout << "\033[" << Cursor << "A";

    clean_animation();//清理屏幕
    Algorithm_Library_Create[Create].Run();
    Algorithm_Library_Search[Search].Run();
    map_Render_Display();//最后一次刷新地图

//记录选择的算法(结算数据时使用)
Select_Create = Create;
Select_Search = Search;

}

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
std::cout <<"Method.[\033[4m"<< Algorithm_Library_Create[Select_Create].Name << "\033[0m]and[\033[4m" << Algorithm_Library_Search[Select_Search].Name << "\033[0m]";
std::cout << "Time used.["<<Time_Duration<<"/s]["<< Xmax << "*" << Ymax << "]" << std::endl;
}
void Maze_AI::exits(std::string Error_message){//错误处理
//std::cout << "[!]Program exited with an error. \n";
std::cout << "\033[31m[!]Error message.-->\033[4m"<< Error_message << "\033[0m";
exit(0);//程序退出
}