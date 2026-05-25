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


void Maze_pathfinding(Maze_AI& maze){//寻找路线(BFS(洪水))
std::queue<int> Pathfinding_data;
int//初始化偏移量(上下左右) 
X_Offset[4]={-1,0,0,+1},
Y_Offset[4]={0,-1,+1,0},
Find_route = 0,History_records_vect,
History_records_child[maze.Y_Map_Max()][maze.X_Map_Max()]={maze.Empty_data_Enum};
Pathfinding_data.push(maze.Obtain_Starting_point());//导入起点
while(!Pathfinding_data.empty()){
int cut = Pathfinding_data.front();
int seaY = cut/maze.X_Map_Max();
int seaX = cut-seaY*maze.X_Map_Max();
int XA_off,YA_off;
Pathfinding_data.pop();
    for(int i = 0;i<4;i++){
        if(Find_route){break;}
        XA_off=seaX+X_Offset[i],YA_off=seaY+Y_Offset[i];
        if(maze.Boundary_check(XA_off,YA_off) && !maze.Tag_Information(XA_off,YA_off,maze.Maze_walls_Enum) && !maze.Tag_Information(XA_off,YA_off,maze.Maze_aipath_Enum)){
            History_records_child[YA_off][XA_off]=(YA_off+(-Y_Offset[i]))*maze.X_Map_Max()+(XA_off+(-X_Offset[i]));//记录路径回溯信息
            if(maze.Check_Destination(XA_off,YA_off)){Find_route=1;break;}//如果找到了             
            maze.Mark_Trace(XA_off,YA_off,maze.Maze_aipath_Enum);
            Pathfinding_data.push(YA_off*maze.X_Map_Max()+XA_off);
            std::stringstream Current_content;
            Current_content << "Search the maze-->[" << XA_off << "," << YA_off << "," << Pathfinding_data.size() <<"]";
            maze.Map_loading(Current_content.str());
        }
    }
    }
History_records_vect=maze.Obtain_destination();
while(true){
int seay = History_records_vect/maze.X_Map_Max();
int seax = History_records_vect-seay*maze.X_Map_Max();
if(maze.Obtain_Starting_point()==seay*maze.X_Map_Max()+seax){break;}
maze.Mark_Trace(seax,seay,maze.Maze_Shortest_route);
std::stringstream Current_content_1;
Current_content_1 << "Shortest path...[" << seax << "," << seay << "]";
maze.Map_loading(Current_content_1.str());   
History_records_vect=History_records_child[seay][seax];
    }
}

void Bidirectional_BFS(Maze_AI& maze){//双向BFS算法(双向BFS)
std::queue<int> Two_way_DBSdata1;//起点数据
std::queue<int> Two_way_DBSdata2;//终点数据
int X_Offset[4]={-1,0,0,+1};//X偏移量
int Y_Offset[4]={0,-1,+1,0};//Y偏移量
int Ymax = maze.Y_Map_Max(),Xmax = maze.X_Map_Max();
int History_records_child_A[Ymax][Xmax];//历史数据点
int History_records_child_B[Ymax][Xmax];//历史数据点
for(int i=0;i<Ymax;i++){
    for(int j=0;j<Xmax;j++){
        History_records_child_A[i][j]=maze.Empty_data_Enum;
        History_records_child_B[i][j]=maze.Empty_data_Enum;
    }
}
int Find_route1=0;
int XA_of_a,YA_of_a,XA_of_b,YA_of_b;
int seayx_a,seayx_b;
Two_way_DBSdata1.push(maze.Obtain_Starting_point());
Two_way_DBSdata2.push(maze.Obtain_destination());//导入终点
while(!(Two_way_DBSdata1.empty()&&Two_way_DBSdata2.empty())){

    
    if(!Two_way_DBSdata1.empty()){
    int seaY1 = Two_way_DBSdata1.front()/Xmax;
    int seaX1 = Two_way_DBSdata1.front()-seaY1*Xmax;
    Two_way_DBSdata1.pop();
    for(int i = 0;i<4;i++){
        XA_of_a = seaX1 + X_Offset[i],YA_of_a = seaY1 + Y_Offset[i];
        if(maze.Boundary_check(XA_of_a,YA_of_a) && maze.Tag_Information(XA_of_a,YA_of_a,maze.Maze_Searching_Traces2))
        {
            Find_route1=1;
            seayx_b = YA_of_a*Xmax+XA_of_a;
            seayx_a = (YA_of_a-Y_Offset[i])*Xmax+(XA_of_a-X_Offset[i]);
            break;
        }
        if(maze.Boundary_check(XA_of_a,YA_of_a) &&  maze.Tag_Information(XA_of_a,YA_of_a,maze.Maze_empty_Enum)){
            History_records_child_A[YA_of_a][XA_of_a]=(YA_of_a+(-Y_Offset[i]))*Xmax+(XA_of_a+(-X_Offset[i]));//记录历史记录
            maze.Mark_Trace(XA_of_a,YA_of_a,maze.Maze_Searching_Traces1);
            Two_way_DBSdata1.push(YA_of_a*Xmax+XA_of_a);
            }
        }}
        if(Find_route1){break;}
    if(!Two_way_DBSdata2.empty()){
    int seaY2 = Two_way_DBSdata2.front()/Xmax;
    int seaX2 = Two_way_DBSdata2.front()-seaY2*Xmax;
    Two_way_DBSdata2.pop();
    for(int i = 0;i<4;i++){
        XA_of_b = seaX2 + X_Offset[i],YA_of_b = seaY2 + Y_Offset[i];
        if(maze.Boundary_check(XA_of_b,YA_of_b) && maze.Tag_Information(XA_of_b,YA_of_b,maze.Maze_Searching_Traces1))
        {
            Find_route1=1;
            seayx_a = YA_of_b*Xmax+XA_of_b;
            seayx_b = (YA_of_b-Y_Offset[i])*Xmax+(XA_of_b-X_Offset[i]);
            break;
        }
        if(maze.Boundary_check(XA_of_b,YA_of_b) &&  maze.Tag_Information(XA_of_b,YA_of_b,maze.Maze_empty_Enum)){
            History_records_child_B[YA_of_b][XA_of_b]=(YA_of_b-Y_Offset[i])*Xmax+(XA_of_b-X_Offset[i]);//记录历史记录
            maze.Mark_Trace(XA_of_b,YA_of_b,maze.Maze_Searching_Traces2);
            Two_way_DBSdata2.push(YA_of_b*Xmax+XA_of_b);
            }
        std::stringstream Current_content;
        Current_content << "Searching[A." << XA_of_a << "," << YA_of_a << "," << Two_way_DBSdata1.size() << "]"<<"[B." << XA_of_b << "," << YA_of_b << "," << Two_way_DBSdata2.size() << "]";
        maze.Map_loading(Current_content.str());
        }}if(Find_route1){break;}
    }


while(maze.Obtain_Starting_point()!=seayx_a || maze.Obtain_destination()!=seayx_b){
int seay1=0,seax1=0,seay2=0,seax2=0;
if(maze.Obtain_Starting_point()!=seayx_a){
seay1 = seayx_a/Xmax;
seax1 = seayx_a - seay1*Xmax;
maze.Mark_Trace(seax1,seay1,maze.Maze_Shortest_route);
seayx_a = History_records_child_A[seay1][seax1];
}
if(maze.Obtain_destination()!=seayx_b){
seay2 = seayx_b/Xmax;
seax2 = seayx_b - seay2*Xmax;
maze.Mark_Trace(seax2,seay2,maze.Maze_Shortest_route);
seayx_b = History_records_child_B[seay2][seax2];
}
std::stringstream Current_content_Path;
Current_content_Path << "Shortest path[" << seax1 << "," << seay1 << "][" << seax2 << "," << seay2 << "]";
maze.Map_loading(Current_content_Path.str());
    }
}

void Search_Algorithm_Algorithm_A(Maze_AI& maze){
struct Recor_G{
int newH;//估测价值
int newG;//实际已走
int newF;//总价值(后期请使用Value_F+Guidewire_H获取)
int Dir_Suo_X;//X位置索引
int Dir_Suo_Y;//Y位置索引
};
struct Historystruct{
int History_Y;//历史记录Y位置
int History_X;//历史记录X位置
};
int Xmax = maze.X_Map_Max(),Ymax = maze.Y_Map_Max();//获取地图大小
int Starting_point = maze.Obtain_Starting_point();//获取起点
int Destination = maze.Obtain_destination();//获取终点
std::vector<Recor_G> Record_List;//记录价值
Historystruct History_List[Ymax][Xmax];//历史记录
int distance[Ymax][Xmax];//距离记录(离终点的距离)
for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        distance[i][j] = Xmax*Ymax;//初始值为一个很大的数
    }
}

int OffsetX[4] = {-1,0,0,1};//偏移量X
int OffsetY[4] = {0,-1,1,0};//偏移量Y

int seaY = Starting_point/Xmax;
int seaX = Starting_point-seaY*Xmax;
distance[seaY][seaX] = 0;//起点距离为0

int seaY1 = Destination/Xmax;
int seaX1 = Destination-seaY1*Xmax;

int poor_road = abs(seaX1 - seaX) + abs(seaY1 - seaY);
int Found_you = 0;

Record_List.push_back({poor_road, 0, poor_road, seaX, seaY});//导入起点
while(Record_List.size()){
int TemMax = Record_List[0]. newF,Tem=0;
for(int i = 0;i<Record_List.size();i++){
    if(TemMax>Record_List[i].newF){
        TemMax=Record_List[i].newF;
        Tem=i;
        }
    }
std::iter_swap(Record_List.begin() + Tem, Record_List.end() - 1);

int PositionY = Record_List[Record_List.size()-1].Dir_Suo_Y;
int PositionX = Record_List[Record_List.size()-1].Dir_Suo_X;
Record_List.pop_back();

    for(int i = 0;i<4;i++){
        int NewY = PositionY + OffsetY[i];
        int NewX = PositionX + OffsetX[i];
    if(maze.Boundary_check(NewX,NewY) && distance[NewY][NewX]>distance[PositionY][PositionX]+1 && !maze.Tag_Information(NewX,NewY,maze.Maze_walls_Enum)){
        maze.Mark_Trace(NewX,NewY,maze.Maze_aipath_Enum);
        History_List[NewY][NewX]={NewY-OffsetY[i],NewX-OffsetX[i]};//记录历史记录
        if(NewY==seaY1 && NewX==seaX1){Found_you = 1; break;}//如果找到了
        distance[NewY][NewX] = distance[PositionY][PositionX]+1;//实际已走距离
        int Guidewire_H = abs(seaX1 - NewX) + abs(seaY1 - NewY);//估测距离
        Record_List.push_back({Guidewire_H, distance[NewY][NewX], Guidewire_H + distance[NewY][NewX], NewX, NewY});
        std::stringstream Current_content;
        Current_content << "Searching...[" << NewX << "," << NewY << "," << Record_List.size() << "|" << distance[NewY][NewX] << "," << Guidewire_H << ","<< Guidewire_H + distance[NewY][NewX] <<"]";
        maze.Map_loading(Current_content.str());
            }
        }
        if(Found_you){break;}
    }
while(Starting_point!=seaY1*Xmax+seaX1){
int History_Y = History_List[seaY1][seaX1].History_Y;
int History_X = History_List[seaY1][seaX1].History_X;
//maze_map[seaY1][seaX1]=Maze_Shortest_route;
maze.Mark_Trace(seaX1,seaY1,maze.Maze_Shortest_route);
seaY1 = History_Y;
seaX1 = History_X;
std::stringstream Current_content;
Current_content << "Shortest path...[" << seaX1 << "," << seaY1 << "]";
maze.Map_loading(Current_content.str());
}

}

void Maze_Path_DP(Maze_AI& maze){//伪动态规划算法实现现(复合算法)
    int Starting_point = maze.Obtain_Starting_point();
    int Destination = maze.Obtain_destination();
    int Xmax = maze.X_Map_Max(),Ymax = maze.Y_Map_Max();//获取地图大小
int distance[Ymax][Xmax];//距离记录(离终点的距离)
for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        distance[i][j] = Xmax*Ymax;//初始值为一个很大的数
    }
}
int PositionY[4] = {-1,0,0,1};//偏移量Y
int PositionX[4] = {0,-1,1,0};//偏移量X
std::queue<int> DPdata;
DPdata.push(Destination);//导入终点
int seaY = Destination/Xmax;
int seaX = Destination-seaY*Xmax;
distance[seaY][seaX] = 0;//起点距离为0
while(DPdata.size()){
seaY = DPdata.front()/Xmax;
seaX = DPdata.front()-seaY*Xmax;
DPdata.pop();
for(int i = 0 ;i<4;i++){
int ScanX = seaX+PositionX[i];
int ScanY = seaY+PositionY[i];
if(maze.Boundary_check(ScanX,ScanY) && !maze.Tag_Information(ScanX,ScanY,maze.Maze_walls_Enum) && !maze.Tag_Information(ScanX,ScanY,maze.Scan_Mark) && distance[ScanY][ScanX]>distance[seaY][seaX]+1){
            distance[ScanY][ScanX] = distance[seaY][seaX]+1;
            DPdata.push(ScanY*Xmax+ScanX);//
            maze.Mark_Trace(ScanX,ScanY,maze.Scan_Mark);
            //maze_map[ScanY][ScanX]=Scan_Mark;
            std::stringstream Current_content;
            Current_content << "PD_Scan map[" << ScanX << "," << ScanY <<","<< DPdata.size() << "]";
            maze.Map_loading(Current_content.str());
            }
        }
    }
//贪心回溯(有问题，待优化(有概率会出现无限循环))
seaY = Starting_point/Xmax;
seaX = Starting_point-seaY*Xmax;
struct Maximum {
int Maximum_value;
int Maximum_Y;
int Maximum_X;
};
Maximum Maximum_data = {distance[seaY][seaX],seaY,seaX};
while(Destination!=seaY*Xmax+seaX){
for(int i = 0;i<4;i++){
    int newX = seaX + PositionX[i];
    int newY = seaY + PositionY[i];
    if(newX<Xmax && newX>=0 && newY<Ymax && newY>=0){
    if(distance[newY][newX]<Maximum_data.Maximum_value){
        Maximum_data = {distance[newY][newX],newY,newX};
        }
    }
}
seaX = Maximum_data.Maximum_X;
seaY = Maximum_data.Maximum_Y;
maze.Mark_Trace(seaX,seaY,maze.Maze_Shortest_route);
//maze_map[seaY][seaX]=Maze_Shortest_route;
std::stringstream Current_content;
Current_content << "Shortest path...[" << seaY << "," << seaX << "]";
maze.Map_loading(Current_content.str());
    }
}//[2026|05|06]伪动态规划算法实现现(复合算法)


void Maze_Path_DP_True(Maze_AI& maze){//动态规划算法实现(真)[v1]
int Starting_point = maze.Obtain_Starting_point();
int Destination = maze.Obtain_destination();
int Xmax = maze.X_Map_Max(),Ymax = maze.Y_Map_Max();//获取地图大小
int Maximum_data = 0;
int distance[Ymax][Xmax];//距离记录(离终点的距离)
for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        distance[i][j] = Xmax*Ymax;//初始值为一个很大的数
    }
}
int PositionY[4] = {-1,0,0,1};//偏移量Y
int PositionX[4] = {0,-1,1,0};//偏移量X
int Cycle = 1;
int time = 0;

int seaY_1 = Destination/Xmax;
int seaX_1 = Destination-seaY_1*Xmax;
distance[seaY_1][seaX_1] = 0;//起点距离为0

while(Cycle){
Cycle = 0;//每次循环开始时重置循环标记
    for(int i = 0;i<Ymax;i++){
        for(int j = 0;j<Xmax;j++){
            if(maze.Tag_Information(j,i,maze.Maze_walls_Enum)){continue;}//如果是墙壁就跳过
            int ScanX;
            int ScanY;

            Maximum_data = distance[i][j];
            for(int k = 0;k<4;k++){//搜索附近四个方向
            ScanX = j+PositionX[k];
            ScanY = i+PositionY[k];
            if(ScanX>=Xmax || ScanX<0 || ScanY>=Ymax || ScanY<0){continue;}//如果越界就跳过
            

                if(distance[i][j] > distance[ScanY][ScanX] + 1){
                    distance[i][j] = distance[ScanY][ScanX] + 1;
                    Cycle = 1;
                    maze.Mark_Trace(j,i,maze.Scan_Mark);
                    //maze_map[i][j] = Scan_Mark;  // 标记这个格子被更新
                    }
                
            }
        }
    }
time++;
std::stringstream Current_content;
Current_content << "DP delivery[" << time << "]times" ;
maze.Map_loading(Current_content.str());
}

int seaY = Starting_point/Xmax;
int seaX = Starting_point-seaY*Xmax;
int temX,temY;
while(Destination!=seaY*Xmax+seaX){

    int less_value = distance[seaY][seaX];
    for(int i = 0;i<4;i++){
        int newX = seaX + PositionX[i];
        int newY = seaY + PositionY[i];
        if(newX<Xmax && newX>=0 && newY<Ymax && newY>=0){
            if(distance[newY][newX]<less_value){
                less_value = distance[newY][newX];
                temX = newX;
                temY = newY;
                //break;
            } 
        }

    }

    seaX = temX;
    seaY = temY;
maze.Mark_Trace(seaX,seaY,maze.Maze_Shortest_route);
    //maze_map[seaY][seaX]=Maze_Shortest_route;

std::stringstream Current_content;
Current_content << "Shortest path...[" << seaX << "," << seaY << "]";
maze.Map_loading(Current_content.str());

}

}//动态规划算法实现[2026|05|08|00:11]


void Bidirectional_A_Algorithm(Maze_AI& maze){//双向A搜索算法
struct Recor_G {
    int newH; //估测价值
    int newG; //实际已走
    int newF; //总价值(后期请使用Value_F+Guidewire_H获取)
    int Dir_Suo_X; //X位置索引
    int Dir_Suo_Y; //Y位置索引
};
struct Historystruct {
    int History_Y; //历史记录Y位置
    int History_X; //历史记录X位置
};
std::vector<Recor_G> Record_List_A; //记录价值
std::vector<Recor_G> Record_List_B; //记录价值
int Xmax = maze.X_Map_Max(), Ymax = maze.Y_Map_Max(); //获取地图大小
Historystruct History_List[Ymax][Xmax]; //历史记录
int distance_a[Ymax][Xmax]; //距离记录(离终点的距离)
int distance_b[Ymax][Xmax];

for (int i = 0; i < Ymax; i++) {
    for (int j = 0; j < Xmax; j++) {
        distance_a[i][j] = Xmax * Ymax; //初始值为一个很大的数
        distance_b[i][j] = Xmax * Ymax;
    }
}

int OffsetX[4] = {-1, 0, 0, 1}; //偏移量X
int OffsetY[4] = {0, -1, 1, 0}; //偏移量Y

// 获取起点和终点索引，并使用函数接口转换为坐标
int Starting_point = maze.Obtain_Starting_point(); // 获取起点位置(索引)
int Destination = maze.Obtain_destination();       // 获取地图终点位置(索引)

int seaY = maze.Get_indexY(Starting_point); //起点Y
int seaX = maze.Get_indexX(Starting_point); //起点X

int seaY1 = maze.Get_indexY(Destination); //终点Y
int seaX1 = maze.Get_indexX(Destination); //终点X

int NewY_b = 0, NewX_b = 0;
int NewY_a = 0, NewX_a = 0;

int Painting_routeX_a = 0, Painting_routeY_a = 0;
int Painting_routeX_b = 0, Painting_routeY_b = 0;

distance_a[seaY][seaX] = 0;   //起点距离为0
distance_b[seaY1][seaX1] = 0; //终点距离为0

int poor_road = abs(seaX1 - seaX) + abs(seaY1 - seaY); //估测(终点/起点)距离
int poor_road_1 = abs(seaX - seaX1) + abs(seaY - seaY1); //估测(起点/终点)距离
int Found_you = 0; //是否找到对方的标记

Record_List_A.push_back({poor_road, 0, poor_road, seaX, seaY}); //导入起点
Record_List_B.push_back({poor_road, 0, poor_road, seaX1, seaY1}); //导入终点

while (Record_List_A.size() || Record_List_B.size()) {

    if (Found_you) {
        break;
    }

    if (Record_List_A.size() && !Found_you) {
        int TemMax_a = Record_List_A[0].newF, Tem_a = 0;
        for (int i = 0; i < Record_List_A.size(); i++) {
            if (TemMax_a > Record_List_A[i].newF) {
                TemMax_a = Record_List_A[i].newF;
                Tem_a = i;
            }
        }
        std::iter_swap(Record_List_A.begin() + Tem_a, Record_List_A.end() - 1);
        int PositionY_A = Record_List_A[Record_List_A.size() - 1].Dir_Suo_Y;
        int PositionX_A = Record_List_A[Record_List_A.size() - 1].Dir_Suo_X;
        Record_List_A.pop_back();

        for (int i = 0; i < 4; i++) {
            NewY_a = PositionY_A + OffsetY[i];
            NewX_a = PositionX_A + OffsetX[i];
            // 使用Boundary_check函数检查边界
            if (!maze.Boundary_check(NewX_a, NewY_a)) {
                continue;
            } //如果越界就跳过

            // 使用Tag_Information函数检查特定的标记
            if (maze.Tag_Information(NewX_a, NewY_a, maze.Maze_Searching_Traces2)) {
                //初始化对方位置
                Painting_routeX_b = NewX_a;
                Painting_routeY_b = NewY_a;
                //初始化自己位置
                Painting_routeX_a = PositionX_A;
                Painting_routeY_a = PositionY_A;
                //History_List[PositionY_A][PositionX_A]={PositionY_A-OffsetY[i],PositionX_A-OffsetX[i]};

                Found_you = 1;
                break;
            } //如果找到了对面
            // 判断条件改为：距离更短 且 不是墙壁
            if (!(distance_a[NewY_a][NewX_a] > distance_a[PositionY_A][PositionX_A] + 1 && !maze.Tag_Information(NewX_a, NewY_a, maze.Maze_walls_Enum))) {
                continue;
            } //如果不满足条件就跳过
            History_List[NewY_a][NewX_a] = {NewY_a - OffsetY[i], NewX_a - OffsetX[i]}; //记录历史记录

            // 使用Mark_Trace函数设置地图标记
            maze.Mark_Trace(NewX_a, NewY_a, maze.Maze_Searching_Traces1);

            distance_a[NewY_a][NewX_a] = distance_a[PositionY_A][PositionX_A] + 1; //实际已走距离
            int Guidewire_H = abs(seaX1 - NewX_a) + abs(seaY1 - NewY_a);             //估测距离
            Record_List_A.push_back({Guidewire_H, distance_a[NewY_a][NewX_a], Guidewire_H + distance_a[NewY_a][NewX_a], NewX_a, NewY_a});
        }
    }
    if (Found_you) {
        break;
    }

    if (Record_List_B.size() && !Found_you) {
        int TemMax_b = Record_List_B[0].newF, Tem_b = 0;
        for (int i = 0; i < Record_List_B.size(); i++) {
            if (TemMax_b > Record_List_B[i].newF) {
                TemMax_b = Record_List_B[i].newF;
                Tem_b = i;
            }
        }
        std::iter_swap(Record_List_B.begin() + Tem_b, Record_List_B.end() - 1);
        int PositionY_B = Record_List_B[Record_List_B.size() - 1].Dir_Suo_Y;
        int PositionX_B = Record_List_B[Record_List_B.size() - 1].Dir_Suo_X;

        Record_List_B.pop_back();

        for (int i = 0; i < 4; i++) {
            NewY_b = PositionY_B + OffsetY[i];
            NewX_b = PositionX_B + OffsetX[i];
            // 使用Boundary_check函数检查边界
            if (!maze.Boundary_check(NewX_b, NewY_b)) {
                continue;
            } //如果越界就跳过

            // 使用Tag_Information函数检查特定的标记
            if (maze.Tag_Information(NewX_b, NewY_b, maze.Maze_Searching_Traces1)) { //如果找到了对面

                //初始化对方位置
                Painting_routeX_a = NewX_b;
                Painting_routeY_a = NewY_b;
                //初始化自己位置
                Painting_routeX_b = PositionX_B;
                Painting_routeY_b = PositionY_B;
                //History_List[PositionY_B][PositionX_B]={PositionY_B-OffsetY[i],PositionX_B-OffsetX[i]};
                Found_you = 1;
                break;
            }
            // 判断条件改为：距离更短 且 不是墙壁
            if (!(distance_b[NewY_b][NewX_b] > distance_b[PositionY_B][PositionX_B] + 1 && !maze.Tag_Information(NewX_b, NewY_b, maze.Maze_walls_Enum))) {
                continue;
            } //如果不满足条件就跳过

            History_List[NewY_b][NewX_b] = {NewY_b - OffsetY[i], NewX_b - OffsetX[i]}; //记录历史记录
            // 使用Mark_Trace函数设置地图标记
            maze.Mark_Trace(NewX_b, NewY_b, maze.Maze_Searching_Traces2);

            distance_b[NewY_b][NewX_b] = distance_b[PositionY_B][PositionX_B] + 1; //实际已走距离
            int Guidewire_H = abs(seaX1 - NewX_b) + abs(seaY1 - NewY_b);             //估测距离
            Record_List_B.push_back({Guidewire_H, distance_b[NewY_b][NewX_b], Guidewire_H + distance_b[NewY_b][NewX_b], NewX_b, NewY_b});
        }
    }
    std::stringstream Current_content;
    Current_content << "Searching...[A|" << NewX_a << "," << NewY_a << "," << Record_List_A.size() << "][B|" << NewX_b << "," << NewY_b << "," << Record_List_B.size() << "]";
    // 使用Map_loading函数刷新地图显示
    maze.Map_loading(Current_content.str());

    //if(Found_you){break;}
}
//seaY seaX起点 seaY1 seaX1终点

while ((Painting_routeX_a != seaX || Painting_routeY_a != seaY) || (Painting_routeX_b != seaX1 || Painting_routeY_b != seaY1)) {

    if (Painting_routeX_b != seaX1 || Painting_routeY_b != seaY1) {
        int History_Y_1 = History_List[Painting_routeY_b][Painting_routeX_b].History_Y;
        int History_X_1 = History_List[Painting_routeY_b][Painting_routeX_b].History_X;
        // 使用Mark_Trace函数设置最短路径标记
        maze.Mark_Trace(Painting_routeX_b, Painting_routeY_b, maze.Maze_Shortest_route);
        Painting_routeY_b = History_Y_1;
        Painting_routeX_b = History_X_1;
    }
    if (Painting_routeX_a != seaX || Painting_routeY_a != seaY) {
        int History_Y_2 = History_List[Painting_routeY_a][Painting_routeX_a].History_Y;
        int History_X_2 = History_List[Painting_routeY_a][Painting_routeX_a].History_X;
        // 使用Mark_Trace函数设置最短路径标记
        maze.Mark_Trace(Painting_routeX_a, Painting_routeY_a, maze.Maze_Shortest_route);
        Painting_routeY_a = History_Y_2;
        Painting_routeX_a = History_X_2;
    }

    std::stringstream Current_content;
    Current_content << "Shortest path...[" << Painting_routeX_b << "," << Painting_routeY_b << "][" << Painting_routeX_a << "," << Painting_routeY_a << "]";
    // 使用Map_loading函数刷新地图显示
    maze.Map_loading(Current_content.str());
    }
}

void Algorithm_A_Revision(Maze_AI& maze){//A*算法的改进版本
struct Recor_G{
int newH;//估测价值
int newG;//实际已走
int newF;//总价值(后期请使用Value_F+Guidewire_H获取)
int Dir_Suo_X;//X位置索引
int Dir_Suo_Y;//Y位置索引
};
struct Historystruct{
int History_Y;//历史记录Y位置
int History_X;//历史记录X位置
};
int Xmax = maze.X_Map_Max(),Ymax = maze.Y_Map_Max();//获取地图大小
int Starting_point = maze.Obtain_Starting_point();//获取起点
int Destination = maze.Obtain_destination();//获取终点
std::vector<Recor_G> Record_List;//记录价值
Historystruct History_List[Ymax][Xmax];//历史记录
int distance[Ymax][Xmax];//距离记录(离终点的距离)
for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        distance[i][j] = Xmax*Ymax;//初始值为一个很大的数
    }
}

int OffsetX[4] = {-1,0,0,1};//偏移量X
int OffsetY[4] = {0,-1,1,0};//偏移量Y

int seaY = Starting_point/Xmax;
int seaX = Starting_point-seaY*Xmax;
distance[seaY][seaX] = 0;//起点距离为0

int seaY1 = Destination/Xmax;
int seaX1 = Destination-seaY1*Xmax;

int poor_road = abs(seaX1 - seaX) + abs(seaY1 - seaY);
int Found_you = 0;

Record_List.push_back({poor_road, 0, poor_road, seaX, seaY});//导入起点

while(Record_List.size()){
int TemMax = Record_List[0]. newF,Tem=0;
int Dead_end = true;//重置如果没路 那么删除(换一个)
for(int i = 0;i<Record_List.size();i++){
    if(TemMax>Record_List[i].newF){
        TemMax=Record_List[i].newF;
        Tem=i;
        }
    }
std::iter_swap(Record_List.begin() + Tem, Record_List.end() - 1);

int PositionY = Record_List[Record_List.size()-1].Dir_Suo_Y;
int PositionX = Record_List[Record_List.size()-1].Dir_Suo_X;
//Record_List.pop_back();

    for(int i = 0;i<4;i++){
        int NewY = PositionY + OffsetY[i];
        int NewX = PositionX + OffsetX[i];
    if(maze.Boundary_check(NewX,NewY) && distance[NewY][NewX]>distance[PositionY][PositionX]+1 && !maze.Tag_Information(NewX,NewY,maze.Maze_walls_Enum)){
        maze.Mark_Trace(NewX,NewY,maze.Maze_aipath_Enum);
        History_List[NewY][NewX]={NewY-OffsetY[i],NewX-OffsetX[i]};//记录历史记录
        if(NewY==seaY1 && NewX==seaX1){Found_you = 1; break;}//如果找到了
        distance[NewY][NewX] = distance[PositionY][PositionX]+1;//实际已走距离
        int Guidewire_H = abs(seaX1 - NewX) + abs(seaY1 - NewY);//估测距离
        Record_List.push_back({Guidewire_H, distance[NewY][NewX], Guidewire_H + distance[NewY][NewX], NewX, NewY});
        std::stringstream Current_content;
        Current_content << "Searching...[" << NewX << "," << NewY << "," << Record_List.size() << "|" << distance[NewY][NewX] << "," << Guidewire_H << ","<< Guidewire_H + distance[NewY][NewX] <<"]";
        maze.Map_loading(Current_content.str());
        Dead_end = false;
            }
        }
        if(Dead_end){Record_List.pop_back();}
        if(Found_you){break;}
        
    }
while(Starting_point!=seaY1*Xmax+seaX1){
int History_Y = History_List[seaY1][seaX1].History_Y;
int History_X = History_List[seaY1][seaX1].History_X;
//maze_map[seaY1][seaX1]=Maze_Shortest_route;
maze.Mark_Trace(seaX1,seaY1,maze.Maze_Shortest_route);
seaY1 = History_Y;
seaX1 = History_X;
std::stringstream Current_content;
Current_content << "Shortest path...[" << seaX1 << "," << seaY1 << "]";
maze.Map_loading(Current_content.str());
    }
}

void Search_Dijkstra_true(Maze_AI& maze){//结构版本[V2.1.0]函数版本[1.0.0](Dijkstra(真))
struct Recor_G{
int new_G;//已走
int x;//X位置索引
int y;//Y位置索引
};
struct History{//历史记录
int x;
int y;
};
std::vector<Recor_G> Recor_G_vect;
int Starting_point = maze.Obtain_Starting_point();
int Destination = maze.Obtain_destination();
int Xmax = maze.X_Map_Max(),Ymax = maze.Y_Map_Max();

History Historymap[Ymax][Xmax];
int distance[Ymax][Xmax];//步数记录

for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        distance[i][j] = Ymax*Xmax;//初始化一个很多的数
    }
}
int offsetX[4] = {-1,0,0,1};//x偏移量
int offsetY[4] = {0,-1,1,0};//y偏移量

int seaY = Starting_point/Xmax;
int seaX = Starting_point-seaY*Xmax;

int destinationY = Destination/Xmax;
int destinationX = Destination - destinationY*Xmax;

int find = false;//是否找到了终点(false/true)

distance[seaY][seaX] = 0;//起点步数为0
Recor_G_vect.push_back({0,seaX,seaY});

while(Recor_G_vect.size()){//主要循环
//每次弹出最小的哪一个
int TemMax = Recor_G_vect[0].new_G,Tem=0;
for(int i = 0;i<Recor_G_vect.size();i++){
    if(TemMax>Recor_G_vect[i].new_G){
        TemMax=Recor_G_vect[i].new_G;Tem=i;
        }
    }

//正式开始处理
int locationX = Recor_G_vect[Tem].x;//x位置
int locationY = Recor_G_vect[Tem].y;//y位置
Recor_G_vect.erase(Recor_G_vect.begin()+Tem);

for(int i=0;i<4;i++){
    int NewY = locationY + offsetY[i];
    int NewX = locationX + offsetX[i];
    if(!maze.Boundary_check(NewX,NewY)){continue;}//超过范围跳过本次循环
    if(maze.Tag_Information(NewX,NewY,maze.Maze_walls_Enum) || distance[NewY][NewX]<=distance[locationY][locationX]+1){continue;}//不符合加入队列条件
    Historymap[NewY][NewX]={locationX,locationY};//记入上一个历史位置
    if(NewY == destinationY && NewX == destinationX){find = true; break;}//如果找到终点 立刻结束循环 并且标记找到    
    maze.Mark_Trace(NewX,NewY,maze.Maze_aipath_Enum);//标记为已搜索路径
    distance[NewY][NewX] = distance[locationY][locationX]+1;//实际已走距离(步数)
    Recor_G_vect.push_back({distance[NewY][NewX],NewX,NewY});//加入处理队列
}
if(find){break;}//检查标记(是否达到了终点)

std::stringstream Current_content;
Current_content << "[Dijkstra]Processing["<< locationX << "," << locationY << "|" << Recor_G_vect.size() <<"]";
maze.Map_loading(Current_content.str());

}
//回溯循环
while(Starting_point!=destinationY*Xmax+destinationX){
int History_Y = Historymap[destinationY][destinationX].y;
int History_X = Historymap[destinationY][destinationX].x;
maze.Mark_Trace(destinationX,destinationY,maze.Maze_Shortest_route);
destinationY = History_Y;
destinationX = History_X;
std::stringstream Current_content;
Current_content << "Shortest path...[" << destinationX << "," << destinationY << "]";
maze.Map_loading(Current_content.str());
    }
}

void Search_Algorithm_JPS(Maze_AI& maze){
struct Recor_G{
int newH;//估测价值
int newG;//实际已走
int newF;//总价值(后期请使用Value_F+Guidewire_H获取)
int Dir_Suo_X;//X位置索引
int Dir_Suo_Y;//Y位置索引
};
struct Historystruct{
int History_Y;//历史记录Y位置
int History_X;//历史记录X位置
};
std::vector<Recor_G> Record_List;//记录价值
int Starting_point = maze.Obtain_Starting_point();
int Destination = maze.Obtain_destination();
int Xmax = maze.X_Map_Max(),Ymax = maze.Y_Map_Max();
Historystruct History_List[Ymax][Xmax];//历史记录
int distance[Ymax][Xmax];//距离记录(离终点的距离)
for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        distance[i][j] = Xmax*Ymax;//初始值为一个很大的数
    }
}

int OffsetX[4] = {0,0,-1,1};//偏移量X
int OffsetY[4] = {-1,1,0,0};//偏移量Y
//上下左右
int seaY = Starting_point/Xmax;
int seaX = Starting_point-seaY*Xmax;
distance[seaY][seaX] = 0;//起点距离为0

int seaY1 = Destination/Xmax;
int seaX1 = Destination-seaY1*Xmax;

int poor_road = abs(seaX1 - seaX) + abs(seaY1 - seaY);
bool Found_you = false;//检查是否到了终点(false/true)
int Guidewire_H;//估测距离临时变量

Record_List.push_back({poor_road, 0, poor_road, seaX, seaY});//导入起点
while(Record_List.size()){
//每次处理最小的哪一个
int TemMax = Record_List[0].newH,Tem=0;
for(int i = 0;i<Record_List.size();i++){
    if(TemMax>Record_List[i].newH){
        TemMax=Record_List[i].newH;Tem=i;
        }
    }

//正式开始处理
int PositionX = Record_List[Tem].Dir_Suo_X;//x位置
int PositionY = Record_List[Tem].Dir_Suo_Y;//y位置
Record_List.erase(Record_List.begin()+Tem);

    for(int i = 0;i<4;i++){
    //处理方向
    int NewY = PositionY;
    int NewX = PositionX;
    
        while(1){
            int lasttime_NewY = NewY;//设置历史记录
            int lasttime_NewX = NewX;
            bool Find_point = false;//是否找到了符合条件的节点

            NewY = NewY + OffsetY[i];//进行移动
            NewX = NewX + OffsetX[i];

            Guidewire_H = abs(seaX1 - NewX) + abs(seaY1 - NewY);//估测距离

            if(!maze.Boundary_check(NewX,NewY) || maze.Tag_Information(NewX,NewY,maze.Maze_walls_Enum) || maze.Tag_Information(NewX,NewY,maze.Maze_aipath_Enum)){//检查是否超过范围以及是不是墙壁(还有自的标记)

            //NewY = lasttime_NewY;
            //NewX = lasttime_NewX;
            break;//退出循环
            }
            History_List[NewY][NewX] = {lasttime_NewY,lasttime_NewX};//记录历史记录

            if(NewY==seaY1 && seaX1==NewX){Found_you=true;break;}//检查是否找到了终点
            
            distance[NewY][NewX] = distance[lasttime_NewY][lasttime_NewX]+1;//记录实际已走距离

            if(OffsetX[i]==0){
            //垂直
            if(maze.Boundary_check(NewX+1,NewY) && maze.Tag_Information(NewX+1,NewY,maze.Maze_empty_Enum)){
               Find_point=true;
            }else if(maze.Boundary_check(NewX-1,NewY) && maze.Tag_Information(NewX-1,NewY,maze.Maze_empty_Enum)){
                Find_point=true;
                }
            }else{
            //水平
            if(maze.Boundary_check(NewX,NewY+1) && maze.Tag_Information(NewX,NewY+1,maze.Maze_empty_Enum)){
               Find_point=true;
            }else if(maze.Boundary_check(NewX,NewY-1) && maze.Tag_Information(NewX,NewY-1,maze.Maze_empty_Enum)){
                Find_point=true;
                }            
            }

            maze.Mark_Trace(NewX,NewY,maze.Maze_aipath_Enum);
            std::stringstream Current_content;
            Current_content << "Searching:" << NewX << "," << NewY;
            maze.Map_loading(Current_content.str());

            if(Find_point){
            Record_List.push_back(
                    {
                        Guidewire_H,//估测距离
                        distance[NewY][NewX],//实际行走
                        Guidewire_H + distance[NewY][NewX],//总价值(估测距离+实际行走)
                        NewX,
                        NewY
                    }
                );
            break;//加入点并退出
            }
            if(Found_you){break;}//如果找到了终点 退出第二个循环
        }

        }
        if(Found_you){break;}
    }
while(Starting_point!=seaY1*Xmax+seaX1){
int History_Y = History_List[seaY1][seaX1].History_Y;
int History_X = History_List[seaY1][seaX1].History_X;
//maze_map[seaY1][seaX1]=Maze_Shortest_route;
maze.Mark_Trace(seaX1,seaY1,maze.Maze_Shortest_route);
seaY1 = History_Y;
seaX1 = History_X;
std::stringstream Current_content;
Current_content << "Shortest path...[" << seaX1 << "," << seaY1 << "]";
maze.Map_loading(Current_content.str());
    }
}
//结构

void Iterative_Deepening(Maze_AI& maze){//迭代深度搜索
int Xmax = maze.X_Map_Max(),Ymax = maze.Y_Map_Max();
struct History{
int x,y;
};
int distance[Ymax][Xmax];//记录步数
int OffsetX[4] = {-1,0,0,1};//x偏移量
int OffsetY[4] = {0,-1,1,0};//y偏移量
History Historymap[Ymax][Xmax];//记录历史记录

int seaY = maze.Obtain_Starting_point()/Xmax;
int seaX = maze.Obtain_Starting_point()-seaY*Xmax;
distance[seaY][seaX] = 0;//起点步数为0
bool Found_you = false;//是否找到了终点(false/true)
int Farewell_mission = 1;
while(!Found_you){

/*优化可以把这一段放到while外面*/
for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        distance[i][j] = Xmax*Ymax;//初始化一个很大的数
    }
}
distance[seaY][seaX] = 0;//起点步数为0

std::vector<int> Processing_Queue;
Processing_Queue.push_back(maze.Obtain_Starting_point());
while(Processing_Queue.size()){
int Handle = Processing_Queue.size()-1;
int Task_X = maze.Get_indexX(Processing_Queue[Handle]);
int Task_Y = maze.Get_indexY(Processing_Queue[Handle]);
bool Delete_task = true;
for(int k = 0;k<4;k++){
int New_X = Task_X + OffsetX[k];
int New_Y = Task_Y + OffsetY[k];

if(!maze.Boundary_check(New_X,New_Y) || distance[New_Y][New_X]>=Farewell_mission){continue;}//超过范围以及送代范围跳过本次循环
if(maze.Tag_Information(New_X,New_Y,maze.Maze_walls_Enum) || distance[New_Y][New_X]<=distance[Task_Y][Task_X]+1){continue;}//不符合加入队列条件
Historymap[New_Y][New_X] = {Task_X,Task_Y};//记录历史记录
if(maze.Check_Destination(New_X,New_Y)){Found_you=true;break;}
distance[New_Y][New_X] = distance[Task_Y][Task_X]+1;//记录实际已走距离
Processing_Queue.push_back(maze.Get_position_index(New_X,New_Y));//加入处理队列
maze.Mark_Trace(New_X,New_Y,maze.Maze_aipath_Enum);//标记地图
Delete_task = false;
std::stringstream Current_content;
Current_content << "Processing[" << New_X << "," << New_Y << "," << Farewell_mission << "|" << Processing_Queue.size() <<"]";
maze.Map_loading(Current_content.str());
}
if(Found_you){break;}
if(Delete_task){Processing_Queue.erase(Processing_Queue.begin()+Handle);}
    }
Farewell_mission++;
}
//回溯循环
int Starting_point = maze.Obtain_Starting_point();
int destinationY = maze.Get_indexY(maze.Obtain_destination());
int destinationX = maze.Get_indexX(maze.Obtain_destination());
while(Starting_point!=destinationY*Xmax+destinationX){
int History_Y = Historymap[destinationY][destinationX].y;
int History_X = Historymap[destinationY][destinationX].x;
maze.Mark_Trace(destinationX,destinationY,maze.Maze_Shortest_route);
destinationY = History_Y;
destinationX = History_X;
std::stringstream Current_content;
Current_content << "Shortest path...[" << destinationX << "," << destinationY << "]";
maze.Map_loading(Current_content.str());
    }

}