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
#include <cstdlib>
//制作时间从上到下日期排序 (有一些可能需要优化) --->

void Maze_AI::Maze_pathfinding(){//寻找路线(BFS(洪水))
std::queue<int> Pathfinding_data;
int//初始化偏移量(上下左右) 
X_Offset[4]={-1,0,0,+1},
Y_Offset[4]={0,-1,+1,0},
Find_route = 0,History_records_vect,
History_records_child[Ymax][Xmax]={Empty_data_Enum};
Pathfinding_data.push(Starting_point);//导入起点
while(!Pathfinding_data.empty()){
int cut = Pathfinding_data.front();
int seaY = cut/Xmax;
int seaX = cut-seaY*Xmax;
int XA_off,YA_off;
Pathfinding_data.pop();
    for(int i = 0;i<4;i++){
        if(Find_route){break;}
        XA_off=seaX+X_Offset[i],YA_off=seaY+Y_Offset[i];
        if(XA_off<Xmax && XA_off>=0 && YA_off<Ymax && YA_off>=0 && (maze_map[YA_off][XA_off]==1 || maze_map[YA_off][XA_off]<0)){
            History_records_child[YA_off][XA_off]=(YA_off+(-Y_Offset[i]))*Xmax+(XA_off+(-X_Offset[i]));//记录路径回溯信息
            if(maze_map[YA_off][XA_off]==Maze_Destination){Find_route=1;break;}//如果找到了             
            maze_map[YA_off][XA_off]=Maze_aipath_Enum; 
            Pathfinding_data.push(YA_off*Xmax+XA_off);
            std::stringstream Current_content;
            Current_content << "Search the maze-->[" << XA_off << "," << YA_off << "," << Pathfinding_data.size() <<"]";
            Map_loading(Current_content.str());
        }
    }
    }
History_records_vect=Destination;
while(1){
int seay = History_records_vect/Xmax;
int seax = History_records_vect-seay*Xmax;
if(Starting_point==seay*Xmax+seax){break;}
maze_map[seay][seax]=Maze_Shortest_route;
std::stringstream Current_content_1;
Current_content_1 << "Shortest path...[" << seax << "," << seay << "]";
Map_loading(Current_content_1.str());
if(History_records_child[seay][seax]==Empty_data_Enum && Destination!=seay*Xmax+seax && Starting_point!=seay*Xmax+seax)
{exits("Maze_data_not_found");}    
History_records_vect=History_records_child[seay][seax];
    }
}

void Maze_AI::Maze_Path_2BFS(){//Bidirectional BFS
std::queue<int> Two_way_DBSdata1;//起点数据
std::queue<int> Two_way_DBSdata2;//终点数据
int X_Offset[4]={-1,0,0,+1};//X偏移量
int Y_Offset[4]={0,-1,+1,0};//Y偏移量
int History_records_child_A[Ymax][Xmax];//历史数据点
int History_records_child_B[Ymax][Xmax];//历史数据点
for(int i=0;i<Ymax;i++){
    for(int j=0;j<Xmax;j++){
        History_records_child_A[i][j]=Empty_data_Enum;
        History_records_child_B[i][j]=Empty_data_Enum;
    }
}
int Find_route1=0;
int XA_of_a,YA_of_a,XA_of_b,YA_of_b;
int seayx_a,seayx_b;
Two_way_DBSdata1.push(Starting_point);
Two_way_DBSdata2.push(Destination);//导入终点
while(!(Two_way_DBSdata1.empty()&&Two_way_DBSdata2.empty())){

    
    if(!Two_way_DBSdata1.empty()){
    int seaY1 = Two_way_DBSdata1.front()/Xmax;
    int seaX1 = Two_way_DBSdata1.front()-seaY1*Xmax;
    Two_way_DBSdata1.pop();
    for(int i = 0;i<4;i++){
        XA_of_a = seaX1 + X_Offset[i],YA_of_a = seaY1 + Y_Offset[i];
        if(XA_of_a<Xmax && XA_of_a>=0 && YA_of_a<Ymax && YA_of_a>=0 && maze_map[YA_of_a][XA_of_a]==Maze_Searching_Traces2)
        {
            Find_route1=1;
            seayx_b = YA_of_a*Xmax+XA_of_a;
            seayx_a = (YA_of_a-Y_Offset[i])*Xmax+(XA_of_a-X_Offset[i]);
            break;
        }
        if(XA_of_a<Xmax && XA_of_a>=0 && YA_of_a<Ymax && YA_of_a>=0 && (maze_map[YA_of_a][XA_of_a]==1 || maze_map[YA_of_a][XA_of_a]<0)){
            History_records_child_A[YA_of_a][XA_of_a]=(YA_of_a+(-Y_Offset[i]))*Xmax+(XA_of_a+(-X_Offset[i]));//记录历史记录
            maze_map[YA_of_a][XA_of_a]=Maze_Searching_Traces1;
            Two_way_DBSdata1.push(YA_of_a*Xmax+XA_of_a);
            //std::stringstream Current_content1;
            //Current_content1 << "Searching(A)...[" << XA_of_a << "," << YA_of_a << "," << Two_way_DBSdata1.size() << "]";
            //Map_loading(Current_content1.str());
            }
        }}if(Find_route1){break;}
    if(!Two_way_DBSdata2.empty()){
    int seaY2 = Two_way_DBSdata2.front()/Xmax;
    int seaX2 = Two_way_DBSdata2.front()-seaY2*Xmax;
    Two_way_DBSdata2.pop();
    for(int i = 0;i<4;i++){
        XA_of_b = seaX2 + X_Offset[i],YA_of_b = seaY2 + Y_Offset[i];
        if(XA_of_b<Xmax && XA_of_b>=0 && YA_of_b<Ymax && YA_of_b>=0 && maze_map[YA_of_b][XA_of_b]==Maze_Searching_Traces1)
        {
            Find_route1=1;
            seayx_a = YA_of_b*Xmax+XA_of_b;
            seayx_b = (YA_of_b-Y_Offset[i])*Xmax+(XA_of_b-X_Offset[i]);
            break;
        }
        if(XA_of_b<Xmax && XA_of_b>=0 && YA_of_b<Ymax && YA_of_b>=0 && (maze_map[YA_of_b][XA_of_b]==1 || maze_map[YA_of_b][XA_of_b]<0)){
            History_records_child_B[YA_of_b][XA_of_b]=(YA_of_b-Y_Offset[i])*Xmax+(XA_of_b-X_Offset[i]);//记录历史记录
            maze_map[YA_of_b][XA_of_b]=Maze_Searching_Traces2;
            Two_way_DBSdata2.push(YA_of_b*Xmax+XA_of_b);
            //std::stringstream Current_content2;
            //Current_content2 << "Searching(B)...[" << XA_of_b << "," << YA_of_b << "," << Two_way_DBSdata2.size() << "]";
            //Map_loading(Current_content2.str());
            }
        std::stringstream Current_content;
        Current_content << "Searching[A." << XA_of_a << "," << YA_of_a << "," << Two_way_DBSdata1.size() << "]"<<"[B." << XA_of_b << "," << YA_of_b << "," << Two_way_DBSdata2.size() << "]";
        Map_loading(Current_content.str());
        }}if(Find_route1){break;}
    }


while(Starting_point!=seayx_a || Destination!=seayx_b){
int seay1=0,seax1=0,seay2=0,seax2=0;
if(Starting_point!=seayx_a){
seay1 = seayx_a/Xmax;
seax1 = seayx_a - seay1*Xmax;
maze_map[seay1][seax1]=Maze_Shortest_route;
seayx_a = History_records_child_A[seay1][seax1];
}
if(Destination!=seayx_b){
seay2 = seayx_b/Xmax;
seax2 = seayx_b - seay2*Xmax;
maze_map[seay2][seax2]=Maze_Shortest_route;
seayx_b = History_records_child_B[seay2][seax2];
}
std::stringstream Current_content_Path;
Current_content_Path << "Shortest path[" << seax1 << "," << seay1 << "][" << seax2 << "," << seay2 << "]";
Map_loading(Current_content_Path.str());
    }
}

void Maze_AI::Search_Algorithm_Algorithm_A(){//A*算法实现
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

Record_List.push_back({poor_road, 0, 0, seaX, seaY});//导入起点
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
    if(NewX<Xmax && NewX>=0 && NewY<Ymax && NewY>=0 && distance[NewY][NewX]>distance[PositionY][PositionX]+1 && maze_map[NewY][NewX]!=Maze_walls_Enum){
        maze_map[NewY][NewX]=Maze_aipath_Enum;
        History_List[NewY][NewX]={NewY-OffsetY[i],NewX-OffsetX[i]};//记录历史记录
        if(NewY==seaY1 && NewX==seaX1){Found_you = 1; break;}//如果找到了
        distance[NewY][NewX] = distance[PositionY][PositionX]+1;//实际已走距离
        int Guidewire_H = abs(seaX1 - NewX) + abs(seaY1 - NewY);//估测距离
        Record_List.push_back({Guidewire_H, distance[NewY][NewX], Guidewire_H + distance[NewY][NewX], NewX, NewY});
        std::stringstream Current_content;
        Current_content << "Searching...[" << NewX << "," << NewY << "," << Record_List.size() << "|" << distance[NewY][NewX] << "," << Guidewire_H << ","<< Guidewire_H + distance[NewY][NewX] <<"]";
        Map_loading(Current_content.str());
            }
        }
        if(Found_you){break;}
    }
while(Starting_point!=seaY1*Xmax+seaX1){
int History_Y = History_List[seaY1][seaX1].History_Y;
int History_X = History_List[seaY1][seaX1].History_X;
maze_map[seaY1][seaX1]=Maze_Shortest_route;
seaY1 = History_Y;
seaX1 = History_X;
std::stringstream Current_content;
Current_content << "Shortest path...[" << seaX1 << "," << seaY1 << "]";
Map_loading(Current_content.str());
}

}//[2026|05|03|15:19]A*算法实现





void Maze_AI::Maze_Path_DP(){//伪动态规划算法实现现(复合算法)
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
if(ScanX<Xmax && ScanX>=0 && ScanY<Ymax && ScanY>=0 && maze_map[ScanY][ScanX]!=Maze_walls_Enum && distance[ScanY][ScanX]>distance[seaY][seaX]+1){
            distance[ScanY][ScanX] = distance[seaY][seaX]+1;
            DPdata.push(ScanY*Xmax+ScanX);
            maze_map[ScanY][ScanX]=Scan_Mark;
            std::stringstream Current_content;
            Current_content << "PD_Scan map[" << ScanX << "," << ScanY <<","<< DPdata.size() << "]";
            Map_loading(Current_content.str());
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
maze_map[seaY][seaX]=Maze_Shortest_route;
std::stringstream Current_content;
Current_content << "Shortest path...[" << seaY << "," << seaX << "]";
Map_loading(Current_content.str());
    }
}//[2026|05|06]伪动态规划算法实现现(复合算法)

void Maze_AI::Maze_Path_DP_Terrain(){//伪动态规划算法实现(复合算法)(地形版)
int distance[Ymax][Xmax];//距离记录(离终点的距离)
//int Gold_coin[Ymax][Xmax] = {0};//金币
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
if(ScanX<Xmax && ScanX>=0 && ScanY<Ymax && ScanY>=0 && maze_map[ScanY][ScanX]!=Maze_walls_Enum && distance[ScanY][ScanX]>distance[seaY][seaX]+1){
            if(maze_map[ScanY][ScanX]==Gold_Coin_Special){
                distance[ScanY][ScanX] = distance[seaY][seaX];
                maze_map[ScanY][ScanX]=Gold_Coin_Tag;
            }else{
                distance[ScanY][ScanX] = distance[seaY][seaX]+1;
                maze_map[ScanY][ScanX]=Scan_Mark;
            }
            
            DPdata.push(ScanY*Xmax+ScanX);
            
            std::stringstream Current_content;
            Current_content << "PD_Scan map[" << ScanX << "," << ScanY <<","<< DPdata.size() << "]";
            Map_loading(Current_content.str());
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
//Maximum_data = {0,0,0};
for(int i = 0;i<4;i++){
    int newX = seaX + PositionX[i];
    int newY = seaY + PositionY[i];
    if(newX<Xmax && newX>=0 && newY<Ymax && newY>=0 && Maze_Shortest_route!=maze_map[newY][newX]){
        if(distance[newY][newX]<Maximum_data.Maximum_value){
        Maximum_data = {distance[newY][newX],newY,newX};
        }
    if(Maximum_data.Maximum_value==distance[newY][newX]){
            Maximum_data = {distance[newY][newX],newY,newX};
    }
            
    }

}
seaX = Maximum_data.Maximum_X;
seaY = Maximum_data.Maximum_Y;
maze_map[seaY][seaX]=Maze_Shortest_route;
std::stringstream Current_content;
Current_content << "Shortest path...[" << seaX << "," << seaY << "]";
Map_loading(Current_content.str());
    }
}



void Maze_AI::Maze_Path_DP_True(){//动态规划算法实现(真)[v1]

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
            if(maze_map[i][j]==Maze_walls_Enum){continue;}
            int ScanX;
            int ScanY;

            Maximum_data = distance[i][j];
            for(int k = 0;k<4;k++){
            ScanX = j+PositionX[k];
            ScanY = i+PositionY[k];
            if(ScanX>=Xmax || ScanX<0 || ScanY>=Ymax || ScanY<0){continue;}//如果越界就跳过
            //如果是墙壁就跳过

                if(distance[i][j] > distance[ScanY][ScanX] + 1){
                    distance[i][j] = distance[ScanY][ScanX] + 1;
                    Cycle = 1;
                    maze_map[i][j] = Scan_Mark;  // 标记这个格子被更新
                    }
                
            }
        }
    }
time++;
std::stringstream Current_content;
Current_content << "DP delivery[" << time << "]times" ;
Map_loading(Current_content.str());
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
maze_map[seaY][seaX]=Maze_Shortest_route;

std::stringstream Current_content;
Current_content << "Shortest path...[" << seaX << "," << seaY << "]";
Map_loading(Current_content.str());

}

}//动态规划算法实现[2026|05|08|00:11]


void Maze_AI::Search_Algorithm_Algorithm_A_Two_way(){//2A*算法实现(双向A*算法实现)(已完成)
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
std::vector<Recor_G> Record_List_A;//记录价值
std::vector<Recor_G> Record_List_B;//记录价值
Historystruct History_List[Ymax][Xmax];//历史记录
int distance_a[Ymax][Xmax];//距离记录(离终点的距离)
int distance_b[Ymax][Xmax];
for(int i = 0;i<Ymax;i++){
    for(int j = 0;j<Xmax;j++){
        
        distance_a[i][j] = Xmax*Ymax;//初始值为一个很大的数
        distance_b[i][j] = Xmax*Ymax;
    }
}

int OffsetX[4] = {-1,0,0,1};//偏移量X
int OffsetY[4] = {0,-1,1,0};//偏移量Y

int seaY = Starting_point/Xmax;//起点Y
int seaX = Starting_point-seaY*Xmax;//起点X

int seaY1 = Destination/Xmax;//终点Y
int seaX1 = Destination-seaY1*Xmax;//终点X

int NewY_b = 0,NewX_b = 0;
int NewY_a = 0,NewX_a = 0;

int Painting_routeX_a = 0,Painting_routeY_a = 0;
int Painting_routeX_b = 0,Painting_routeY_b = 0;


distance_a[seaY][seaX] = 0;//起点距离为0
distance_b[seaY1][seaX1] = 0;//终点距离为0

int poor_road = abs(seaX1 - seaX) + abs(seaY1 - seaY);//估测(终点/起点)距离

int Found_you = 0;//是否找到对方的标记

Record_List_A.push_back({poor_road, 0, 0, seaX, seaY});//导入起点
Record_List_B.push_back({poor_road, 0, 0, seaX1, seaY1});//导入终点


while(Record_List_A.size() || Record_List_B.size()){

    if(Found_you){break;}

if(Record_List_A.size()&& !Found_you){
int TemMax_a = Record_List_A[0].newF,Tem_a=0;
for(int i = 0;i<Record_List_A.size();i++){
    if(TemMax_a>Record_List_A[i].newF){
        TemMax_a=Record_List_A[i].newF;
        Tem_a=i;
        }
    }
std::iter_swap(Record_List_A.begin() + Tem_a, Record_List_A.end() - 1);
int PositionY_A = Record_List_A[Record_List_A.size()-1].Dir_Suo_Y;
int PositionX_A = Record_List_A[Record_List_A.size()-1].Dir_Suo_X;
Record_List_A.pop_back();

    for(int i = 0;i<4;i++){
        NewY_a = PositionY_A + OffsetY[i];
        NewX_a = PositionX_A + OffsetX[i];
    if(NewX_a>=Xmax || NewX_a<0 || NewY_a>=Ymax || NewY_a<0){continue;}//如果越界就跳过
    
    if(maze_map[NewY_a][NewX_a]==Maze_Searching_Traces2){
        //初始化对方位置
        Painting_routeX_b = NewX_a;
        Painting_routeY_b = NewY_a;
        //初始化自己位置
        Painting_routeX_a = PositionX_A;
        Painting_routeY_a = PositionY_A;
        //History_List[PositionY_A][PositionX_A]={PositionY_A-OffsetY[i],PositionX_A-OffsetX[i]};

        Found_you = 1; 
        break;
    }//如果找到了对面
    if(!(distance_a[NewY_a][NewX_a]>distance_a[PositionY_A][PositionX_A]+1 && maze_map[NewY_a][NewX_a]!=Maze_walls_Enum)){continue;}//如果不满足条件就跳过
    History_List[NewY_a][NewX_a]={NewY_a-OffsetY[i],NewX_a-OffsetX[i]};//记录历史记录
        
        
        maze_map[NewY_a][NewX_a]=Maze_Searching_Traces1;
        

        distance_a[NewY_a][NewX_a] = distance_a[PositionY_A][PositionX_A]+1;//实际已走距离
        int Guidewire_H = abs(seaX - NewX_a) + abs(seaY - NewY_a);//估测距离
        Record_List_A.push_back({Guidewire_H, distance_a[NewY_a][NewX_a], Guidewire_H + distance_a[NewY_a][NewX_a], NewX_a, NewY_a});
        }
}
        if(Found_you){break;}


if(Record_List_B.size()&& !Found_you){
int TemMax_b = Record_List_B[0].newF,Tem_b=0;
for(int i = 0;i<Record_List_B.size();i++){
    if(TemMax_b>Record_List_B[i].newF){
        TemMax_b=Record_List_B[i].newF;
        Tem_b=i;
        }
    }
std::iter_swap(Record_List_B.begin() + Tem_b, Record_List_B.end() - 1);
int PositionY_B = Record_List_B[Record_List_B.size()-1].Dir_Suo_Y;
int PositionX_B = Record_List_B[Record_List_B.size()-1].Dir_Suo_X;

Record_List_B.pop_back();

    for(int i = 0;i<4;i++){
        NewY_b = PositionY_B + OffsetY[i];
        NewX_b = PositionX_B + OffsetX[i];
    if(NewX_b>=Xmax || NewX_b<0 || NewY_b>=Ymax || NewY_b<0){continue;}//如果越界就跳过

    if(maze_map[NewY_b][NewX_b]==Maze_Searching_Traces1){//如果找到了对面

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
    if(!(distance_b[NewY_b][NewX_b]>distance_b[PositionY_B][PositionX_B]+1 && maze_map[NewY_b][NewX_b]!=Maze_walls_Enum)){continue;}//如果不满足条件就跳过
    
        
        History_List[NewY_b][NewX_b]={NewY_b-OffsetY[i],NewX_b-OffsetX[i]};//记录历史记录
        maze_map[NewY_b][NewX_b]=Maze_Searching_Traces2;
        

        distance_b[NewY_b][NewX_b] = distance_b[PositionY_B][PositionX_B]+1;//实际已走距离
        int Guidewire_H = abs(seaX1 - NewX_b) + abs(seaY1 - NewY_b);//估测距离
        Record_List_B.push_back({Guidewire_H, distance_b[NewY_b][NewX_b], Guidewire_H + distance_b[NewY_b][NewX_b], NewX_b, NewY_b});
        }
}        
        std::stringstream Current_content;
        Current_content << "Searching...[A|" << NewX_a << "," << NewY_a << "," << Record_List_A.size() << "][B|" << NewX_b << "," << NewY_b << "," << Record_List_B.size() << "]";
        Map_loading(Current_content.str());

        //if(Found_you){break;}

    }
//seaY seaX起点 seaY1 seaX1终点

while((Painting_routeX_a!=seaX||Painting_routeY_a!=seaY) || (Painting_routeX_b!=seaX1||Painting_routeY_b!=seaY1)){

if(Painting_routeX_b!=seaX1||Painting_routeY_b!=seaY1){
int History_Y_1 = History_List[Painting_routeY_b][Painting_routeX_b].History_Y;
int History_X_1 = History_List[Painting_routeY_b][Painting_routeX_b].History_X;
maze_map[Painting_routeY_b][Painting_routeX_b]=Maze_Shortest_route;
Painting_routeY_b = History_Y_1;
Painting_routeX_b = History_X_1;
}
if(Painting_routeX_a!=seaX||Painting_routeY_a!=seaY){
int History_Y_2 = History_List[Painting_routeY_a][Painting_routeX_a].History_Y;
int History_X_2 = History_List[Painting_routeY_a][Painting_routeX_a].History_X;
maze_map[Painting_routeY_a][Painting_routeX_a]=Maze_Shortest_route;
Painting_routeY_a = History_Y_2;
Painting_routeX_a = History_X_2;
}


std::stringstream Current_content;
Current_content << "Shortest path...[" << Painting_routeX_b << "," << Painting_routeY_b << "][" << Painting_routeX_a << "," << Painting_routeY_a << "]";
Map_loading(Current_content.str());


    }

}//[2026|05|09|00:00]双向A*算法实现