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
std::uniform_int_distribution<int> dist(0,(maze.X_Map_Max()*maze.Y_Map_Max()-1));
maze.Set_Starting_point(dist(rng));
Search_data.push_back(maze.Obtain_Starting_point());
while(Search_data.size()){
std::shuffle(Guide, Guide + 4, rng);//随机偏移量
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
}

void generateMazeByPrim(Maze_AI& maze){//生成迷宫(Prim算法)
int Xmax = maze.X_Map_Max();//地图最大X
int Ymax = maze.Y_Map_Max();//地图最大Y
int Guide[4]={0,1,2,3};//偏移量编号
int XOffset[4]={-2,0,0,2};//x偏移量
int YOffset[4]={0,-2,2,0};//y偏移量
int Finish;//暂时设置终点
std::vector<int> Search_data;//处理队列

std::random_device rd;// 获取真随机数种子
std::mt19937 rng(rd());//创建随机数引擎
std::uniform_int_distribution<int> dist(0,(Xmax*Ymax-1));
maze.Set_Starting_point(dist(rng));
Search_data.push_back(maze.Obtain_Starting_point());//加入起点
while(Search_data.size()){
int Dead_end = true;//如果没路
std::shuffle(Guide, Guide + 4, rng);//随机偏移量
int idx = rng() % Search_data.size();//随机取一个
int Original_X = maze.Get_indexX(Search_data[idx]);
int Original_Y = maze.Get_indexY(Search_data[idx]);
int Handle_X,Handle_Y;

maze.Mark_Trace(Original_X, Original_Y, maze.Maze_empty_Enum);//标记当前位置
for(int i=0;i<4;i++){
Handle_X = Original_X + XOffset[Guide[i]];
Handle_Y = Original_Y + YOffset[Guide[i]];

if(maze.Boundary_check(Handle_X,Handle_Y) && !maze.Location_Marker(Handle_X,Handle_Y)){
    maze.Mark_Trace(Handle_X-(XOffset[Guide[i]]/2),Handle_Y-(YOffset[Guide[i]]/2),maze.Maze_empty_Enum);
    maze.Mark_Trace(Handle_X,Handle_Y,maze.Maze_empty_Enum);
    Finish = Handle_Y*Xmax+Handle_X;//暂时设置终点
    Search_data.push_back(Handle_Y*Xmax+Handle_X);//加入下一个任务

    maze.Log_stream << "Create a maze-->[" << Handle_X << "," << Handle_Y << "," << Search_data.size() <<"]";
    maze.Map_loading(maze.Log_stream.str());
    Dead_end = false;
    break;//只找一个方向就退出循环
        }
    }
    if(Dead_end){
    Search_data.erase(Search_data.begin()+idx);//删除这个任务    
    }
}
maze.Set_destination(Finish);//设置终点

}

void Recursive_Division(Maze_AI& maze){//生成迷宫(递归分割)
int Xmax = maze.X_Map_Max()-1;
int Ymax = maze.Y_Map_Max()-1;

struct Rect {
    int left;   // 左上角 X
    int top;    // 左上角 Y

    int right;  // 右下角 X
    int bottom; // 右下角 Y
};

std::vector<Rect> Blocks;
Blocks.push_back({0,0,Xmax,Ymax});
while(Blocks.size()){
int blosize = maze.Random_number(0,Blocks.size()-1);

int left = Blocks[blosize].left;// 左上角 X
int top = Blocks[blosize].top;// 左上角 Y

int right = Blocks[blosize].right;// 右下角 X
int bottom = Blocks[blosize].bottom;// 右下角 Y


int width  = right - left + 1; // 宽
int height = bottom - top + 1; // 高


if (width >= 3 && height >= 3){
if(width > height){// 垂直切

int tangent_point_X = maze.Random_number(left+1,right-1);//获取垂直切割点(X)

for(int i=top+1;i<=bottom-1;i++){
    maze.Mark_Trace(tangent_point_X,i,maze.Maze_walls_Enum);//切开(墙壁)
}
int Open_thedoor_Y = maze.Random_number(top + 1,bottom -1);//开门的位置(垂直)(Y)
maze.Mark_Trace(tangent_point_X, Open_thedoor_Y, maze.Maze_empty_Enum);//开门

Blocks.push_back({//分裂块
    left,
    top,
    tangent_point_X-1,
    bottom
});
Blocks.push_back({
    tangent_point_X+1,
    top,
    right,
    bottom
});

maze.Log_stream << "Processing[" << left << "," << top << "," << right << "," << bottom << "]";
maze.Map_loading(maze.Log_stream.str());

}else{// 水平切

int tangent_point_Y = maze.Random_number(top+1,bottom-1);//获取水平切割点(X)

for(int i=left+1;i<=right-1;i++){
    maze.Mark_Trace(i,tangent_point_Y,maze.Maze_walls_Enum);//切开(墙壁)
}
int Open_thedoor_X = maze.Random_number(left + 1,right - 1);//开门的位置(水平)(Y)
maze.Mark_Trace(Open_thedoor_X, tangent_point_Y, maze.Maze_empty_Enum);//开门
//分裂块
    Blocks.push_back({
            left,
            top,
            right,
            tangent_point_Y - 1
        });
    Blocks.push_back({
            left,
            tangent_point_Y + 1,
            right,
            bottom
        });
    }

maze.Log_stream << "Processing[" << left << "," << top << "," << right << "," << bottom << "]";
maze.Map_loading(maze.Log_stream.str());

}


//删除块
Blocks.erase(Blocks.begin()+blosize);
}
while(true){//随机起点
int Starting_point_X = maze.Random_number(0,Xmax);
int Starting_point_Y = maze.Random_number(0,Ymax);
if(maze.Location_Marker(Starting_point_X, Starting_point_Y) == maze.Maze_empty_Enum){
maze.Set_Starting_point(Starting_point_Y*maze.X_Map_Max()+Starting_point_X);
break;
    }
}
while(true){//防止随机起点终点重合

while(true){//随机终点
int Destination_point_X = maze.Random_number(0,Xmax);
int Destination_point_Y = maze.Random_number(0,Ymax);
if(maze.Location_Marker(Destination_point_X, Destination_point_Y) == maze.Maze_empty_Enum){
maze.Set_destination(Destination_point_Y*maze.X_Map_Max()+Destination_point_X);
break;
            }
        }
    if(maze.Obtain_destination() != maze.Obtain_Starting_point()){
        break;//终点和起点不重合
    }
    }
}

void MST_Minimum_Spanning_Tree(Maze_AI& maze){//生成迷宫(最小生成树)
int Xmax = maze.X_Map_Max();
int Ymax = maze.Y_Map_Max();
struct PoinT {
bool Room = false;//是否为房间
int ID;
};
struct WalL{
int x;
int y;
};
struct PoinT_Handle{
int x;
int y;
int id;
};

PoinT Points[Ymax][Xmax];//初始化房间
std::vector<WalL> Walls;
bool RoomArray[Ymax][Xmax] = {false};//是否为房间
bool interval_Y = true;
bool interval_X = true;
for(int i=0,id = 0;i<Ymax;i++){
    interval_Y = !interval_Y;
    if(interval_Y){
    for(int j=0;j<Xmax;j++){
        interval_X = !interval_X;
        if(interval_X){
            maze.Mark_Trace(j,i,maze.Maze_empty_Enum);
            RoomArray[i][j] = true;
            Points[i][j] = {true,id};
            id++;
            }
        }
    }
}
int XOffset[4]={-1,0,1,0};
int YOffset[4]={0,-1,0,1};

for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        if(RoomArray[i][j]){//如果是房间
            continue;//跳过
        }
        int Room_walls = 0;
        for(int k = 0;k<4;k++){
            if(maze.Boundary_check(j+XOffset[k],i+YOffset[k]) && RoomArray[i+YOffset[k]][j+XOffset[k]]){
            Room_walls++;
            }
            if(Room_walls == 2){
                Walls.push_back({j,i});
                continue;//提前退出
            }
        }
    }
}

std::random_device rd;  // 获取真随机数种子
std::mt19937 rng(rd());   // 创建随机数引擎
std::shuffle(Walls.begin(), Walls.end(), rng);
bool Starting_point_flag = true;
int Temporary_endpoint;
while(Walls.size()){
int Handle = Walls.size()-1;
int Handle_X = Walls[Handle].x;
int Handle_Y = Walls[Handle].y;
Walls.erase(Walls.begin()+Handle);//删除


std::vector<PoinT_Handle> Point_id;//初始化
for(int k = 0;k<4;k++){
    int Next_X = Handle_X + XOffset[k];
    int Next_Y = Handle_Y + YOffset[k];
    if(!maze.Boundary_check(Next_X,Next_Y)){continue;}
    if(RoomArray[Next_Y][Next_X]){
        Point_id.push_back({Next_X,Next_Y,Points[Next_Y][Next_X].ID});
    }
}
if(Point_id[0].id != Point_id[1].id){
maze.Mark_Trace(Handle_X,Handle_Y,maze.Maze_empty_Enum);
int Cover_min = std::min(Point_id[0].id,Point_id[1].id);//获取最小id
int Cover_max = std::max(Point_id[0].id,Point_id[1].id);//获取最大id
for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        if(Points[i][j].Room){
            if(Points[i][j].ID == Cover_max){
                Points[i][j].ID = Cover_min;
                }
            }
        }
    }

Temporary_endpoint = Point_id[0].y*Xmax+Point_id[0].x;
//刷新地图
std::stringstream Current_content;
Current_content << "Create a maze[" << Handle_X << "," << Handle_Y << "," << Walls.size() <<"]";
maze.Map_loading(Current_content.str());

}

if(Starting_point_flag){//第一次设置起点
    maze.Set_Starting_point(Handle_Y*Xmax+Handle_X);
    Starting_point_flag = false;
}

}
//设置终点
maze.Set_destination(Temporary_endpoint);

}