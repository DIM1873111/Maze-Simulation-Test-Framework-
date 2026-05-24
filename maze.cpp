
/*
           _______  ______  _______________
          /  ___  \ \____/ /  ___   ___   /
         /  /   |  | ___  /  /  /  /  /  /
        /  /   /  / /  / /  /  /  /  /  /
       /  /   /  / /  / /  /  /  /  /  /
      /  /__ /  / /  / /  /  /  /  /  /
     /_________/ /__/ /__/  /__/  /__/
    Maze_AI_Project_start_time[20260427]
*/

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
#include <SFML/Graphics.hpp>//SFML库(图形库)
#include <fstream>
#include "json.hpp"//json库

Maze_AI::Maze_AI(){


std::string Config_Document = "config.json";//获取配置文件
nlohmann::json Config;
std::ifstream Document(Config_Document);

if(Document.is_open()){
    Document >> Config;
    Document.close();
    map_Smallest = Config.value("map_Smallest",map_Smallest);//最小地图尺寸
    Map_Maximum = Config.value("Map_Maximum",Map_Maximum);//最大地图尺寸
    Xmax = Config.value("DefaultXmax",Xmax);//默认地图最大X尺寸
    Ymax = Config.value("DefaultYmax",Ymax);//默认地图最大Y尺寸
    Proportion = Config.value("Proportion",Proportion);//缩放比例
    Rendering_speed = Config.value("Rendering_speed",Rendering_speed);//默认渲染速度
    Minimum_speed = Config.value("Minimum_speed",Minimum_speed);//最快渲染速度
    Maximum_speed = Config.value("Maximum_speed",Maximum_speed);//最慢渲染速度
    Speed_Adjustment = Config.value("Speed_Adjustment",Speed_Adjustment);//渲染速度调节(大小)
    Adjust_Zoom = Config.value("Adjust_Zoom",Adjust_Zoom);//超时自动调整缩放比例
    End_waiting = Config.value("End_waiting",End_waiting);//结束等待时间
//读取颜色配置(并设置默认值)
nlohmann::json 
Maze_Destination_colorsArray = Config.value("Maze_Destination_colors", nlohmann::json({Maze_Destination_colors.r,Maze_Destination_colors.g,Maze_Destination_colors.b})),
Maze_Starting_point_colorsArray = Config.value("Maze_Starting_point_colors", nlohmann::json({Maze_Starting_point_colors.r,Maze_Starting_point_colors.g,Maze_Starting_point_colors.b})),
Maze_aipath_colorsArray = Config.value("Maze_aipath_colors", nlohmann::json({Maze_aipath_colors.r,Maze_aipath_colors.g,Maze_aipath_colors.b})),
Maze_Shortest_route_colorsArray = Config.value("Maze_Shortest_route_colors", nlohmann::json({Maze_Shortest_route_colors.r,Maze_Shortest_route_colors.g,Maze_Shortest_route_colors.b})),
Maze_Searching_Traces1_colorsArray = Config.value("Maze_Searching_Traces1_colors", nlohmann::json({Maze_Searching_Traces1_colors.r,Maze_Searching_Traces1_colors.g,Maze_Searching_Traces1_colors.b})),
Maze_Searching_Traces2_colorsArray = Config.value("Maze_Searching_Traces2_colors", nlohmann::json({Maze_Searching_Traces2_colors.r,Maze_Searching_Traces2_colors.g,Maze_Searching_Traces2_colors.b})),
Gold_Coin_Special_colorsArray = Config.value("Gold_Coin_Special_colors", nlohmann::json({Gold_Coin_Special_colors.r,Gold_Coin_Special_colors.g,Gold_Coin_Special_colors.b})),
Scan_Mark_colorsArray = Config.value("Scan_Mark_colors", nlohmann::json({Scan_Mark_colors.r,Scan_Mark_colors.g,Scan_Mark_colors.b})),
Gold_Coin_Tag_colorsArray = Config.value("Gold_Coin_Tag_colors", nlohmann::json({Gold_Coin_Tag_colors.r,Gold_Coin_Tag_colors.g,Gold_Coin_Tag_colors.b})),
Maze_empty_colorsArray = Config.value("Maze_empty_colors", nlohmann::json({Maze_empty_colors.r,Maze_empty_colors.g,Maze_empty_colors.b})),
Maze_walls_colorsArray = Config.value("Maze_walls_colors", nlohmann::json({Maze_walls_colors.r,Maze_walls_colors.g,Maze_walls_colors.b})),
Maze_unknown_colorsArray = Config.value("Maze_unknown_colors", nlohmann::json({Maze_unknown_colors.r,Maze_unknown_colors.g,Maze_unknown_colors.b}));
//获取颜色配置
Maze_Destination_colors = {createColorFromJson(Maze_Destination_colorsArray)},
Maze_Starting_point_colors = (createColorFromJson(Maze_Starting_point_colorsArray)),
Maze_aipath_colors = (createColorFromJson(Maze_aipath_colorsArray)),
Maze_Shortest_route_colors = (createColorFromJson(Maze_Shortest_route_colorsArray)),
Maze_Searching_Traces1_colors = (createColorFromJson(Maze_Searching_Traces1_colorsArray)),
Maze_Searching_Traces2_colors = (createColorFromJson(Maze_Searching_Traces2_colorsArray)),
Scan_Mark_colors = (createColorFromJson(Scan_Mark_colorsArray)),
Gold_Coin_Tag_colors = (createColorFromJson(Gold_Coin_Tag_colorsArray)),
Maze_empty_colors = (createColorFromJson(Maze_empty_colorsArray)),
Maze_walls_colors = (createColorFromJson(Maze_walls_colorsArray)),
Maze_unknown_colors = (createColorFromJson(Maze_unknown_colorsArray));

}else{//配置文件不存在
Config = {
{"map_Smallest",map_Smallest},//最小地图尺寸
{"Map_Maximum",Map_Maximum},//最大地图尺寸
{"DefaultXmax",Xmax},//默认地图最大X尺寸
{"DefaultYmax",Ymax},//默认地图最大Y尺寸
{"Proportion",Proportion},//缩放比例
{"Rendering_speed",Rendering_speed},//默认渲染速度
{"Minimum_speed",Minimum_speed},//最快渲染速度
{"Maximum_speed",Maximum_speed},//最慢渲染速度
{"Speed_Adjustment",Speed_Adjustment},//渲染速度调节(大小)
{"Adjust_Zoom",Adjust_Zoom},//超时自动调整缩放比例
{"End_waiting",End_waiting},//结束等待时间

{"Maze_Destination_colors",{Maze_Destination_colors.r,Maze_Destination_colors.g,Maze_Destination_colors.b}},
{"Maze_Starting_point_colors",{Maze_Starting_point_colors.r,Maze_Starting_point_colors.g,Maze_Starting_point_colors.b}},
{"Maze_aipath_colors",{Maze_aipath_colors.r,Maze_aipath_colors.g,Maze_aipath_colors.b}},
{"Maze_Shortest_route_colors",{Maze_Shortest_route_colors.r,Maze_Shortest_route_colors.g,Maze_Shortest_route_colors.b}},
{"Maze_Searching_Traces1_colors",{Maze_Searching_Traces1_colors.r,Maze_Searching_Traces1_colors.g,Maze_Searching_Traces1_colors.b}},
{"Maze_Searching_Traces2_colors",{Maze_Searching_Traces2_colors.r,Maze_Searching_Traces2_colors.g,Maze_Searching_Traces2_colors.b}},
{"Gold_Coin_Special_colors",{Gold_Coin_Special_colors.r,Gold_Coin_Special_colors.g,Gold_Coin_Special_colors.b}},
{"Scan_Mark_colors",{Scan_Mark_colors.r,Scan_Mark_colors.g,Scan_Mark_colors.b}},
{"Gold_Coin_Tag_colors",{Gold_Coin_Tag_colors.r,Gold_Coin_Tag_colors.g,Gold_Coin_Tag_colors.b}},
{"Maze_empty_colors",{Maze_empty_colors.r,Maze_empty_colors.g,Maze_empty_colors.b}},
{"Maze_walls_colors",{Maze_walls_colors.r,Maze_walls_colors.g,Maze_walls_colors.b}},
{"Maze_unknown_colors",{Maze_unknown_colors.r,Maze_unknown_colors.g,Maze_unknown_colors.b}}

};
std::ofstream OutFile(Config_Document);//创建配置文件
if (OutFile.is_open()){
    OutFile << std::setw(4) << Config << std::endl;
    OutFile.close();
}else{
    //调用日志
    Log_output("Failed to create the configuration file",Log_Error);
    //exits("Failed to create the configuration file",Exit_early);
    }
}
mapMax = Xmax*Ymax;
}

Maze_AI::~Maze_AI(){
    /*如果需要清理资源，在这里写*/
}
Maze_AI::Map_data_colors Maze_AI::createColorFromJson(const nlohmann::json& colorArray){
    Map_data_colors color = {colorArray[0], colorArray[1], colorArray[2]};
    return color;
}

//提供添加算法仓库函数
void Maze_AI::add_Create(std::string Name,std::string Introduction,std::function<void()> Run,int Type,Mazemapdata Initialization){//添加生成算法仓库
Algorithm_Library_Create.push_back(
    {
        std::move(Name),//名字
        std::move(Introduction),//介绍
        std::move(Run),//函数地址
        Type,//类型
        Initialization//初始化标记
    }
);
}
void Maze_AI::add_Search(std::string Name,std::string Introduction,std::function<void()> Run,int Type){//添加搜索算法仓库
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

void Maze_AI::End_Point(){//终起xy位置初始化
Starting_point_Y = Starting_point/Xmax;
Starting_point_X = Starting_point-Starting_point_Y*Xmax;

Destination_Y = Destination/Xmax;
Destination_X = Destination-Destination_Y*Xmax;

Mark_Trace(
    Get_indexX(Obtain_destination()),
    Get_indexY(Obtain_destination()),
    Maze_Destination
);

Mark_Trace(
    Get_indexX(Obtain_Starting_point()),
    Get_indexY(Obtain_Starting_point()),
    Maze_Starting_point
);
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

Algorithm_selection(sea,cre);
}else if(Parameter >= 2){
cout_title();
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
    cout_title();
    //std::cout << "[Dim|C++]Maze_Simulation_\033[4m" << Project_Version << "\033[0m" << std::endl;
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
   Algorithm_selection(Generation,Pathfinding);
}

//主要渲染函数 

void Maze_AI::Processing_window(){//处理窗口事件以及更新数据
while (const std::optional<sf::Event> event = window.pollEvent()){
    if(event->is<sf::Event::Closed>()){//如果点击了窗口关闭
        window.close();
        Log_stream << "Window closed, stop the program";
        Log_output(Log_stream.str(),Log_Warning);
        exits("Program closed",Exit_early);    
        }
        if(!window.isOpen()){//窗口意外关闭
            Log_stream << "The window was closed";
            Log_output(Log_stream.str(),Log_Warning);
        exits("Program closed",Exit_early);
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){//如果按下了键盘
        switch (keyPressed->code)
        {
        case sf::Keyboard::Key::Space:
            Pause_Status = !Pause_Status;
            Log_stream << "Paused state[" << Pause_Status << "]";
            Log_output(Log_stream.str(),Log_Warning);
            break;
        case sf::Keyboard::Key::Escape:
            ESC_Exit_Status = true;
            break;
        case sf::Keyboard::Key::Hyphen:
            Accelerate_Status = true;
            break;
        case sf::Keyboard::Key::Equal:
            Decelerate_Status = true;
            break;

        default:
            break;
        }

        }
    }
}

void Maze_AI::Interactive_State_window(){
while(true){//暂停功能循环

    if(ESC_Exit_Status){
        window.close();
        Log_stream << "ESC closes the window";
        Log_output(Log_stream.str(),Log_Warning);
        exits("Program closed",Exit_early);
    }

    if (Accelerate_Status){
        Rendering_speed = Rendering_speed+Speed_Adjustment;//渲染速度加快
        if(Rendering_speed>Maximum_speed){Rendering_speed = Maximum_speed;}//最大渲染速度限制
        Log_stream << "Speed Adjustment[" << Rendering_speed << "]";
        Log_output(Log_stream.str(),Log_Warning);
        Accelerate_Status = false;
    }

    if (Decelerate_Status){
        Rendering_speed = Rendering_speed-Speed_Adjustment;//渲染速度加慢
        if(Rendering_speed<Minimum_speed){Rendering_speed = Minimum_speed;}//最小渲染速度限制
        Log_stream << "Speed Adjustment[" << Rendering_speed << "]";
        Log_output(Log_stream.str(),Log_Warning);
        Decelerate_Status = false;
    }

    if (Pause_Status){//暂停功能
    Pause_Status = true;//开始暂停
    }else if (!Pause_Status){
    Pause_Status = false;//解除暂停
    }

    if(!Pause_Status){break;}//没有暂停退出循环
    sf::sleep(sf::milliseconds(10));//检查间隔
    Processing_window();//处理窗口数据(刷新状态)
    }
}


void Maze_AI::Map_loading(std::string mapdata){//迷宫渲染
int Time_Current = Get_the_time();

Time_Duration = Time_Current - Start_Time;

sf::RectangleShape square;//方块(地图数据)

for(int i = 0;i<Ymax;i++){//导入显示数据
    for(int j = 0;j<Xmax;j++){
        Render_cout(i,j);//写入地图数据
        }
    }
    //渲染地图
Processing_window();//窗口刷新(处理窗口关闭等情况)
Interactive_State_window();//处理交互信息
sf::sleep(sf::milliseconds(Rendering_speed));//渲染间隔
for(int i = 0;i<Ymax;i++){//渲染
    for(int j = 0;j<Xmax;j++){
        square.setSize(sf::Vector2f(Rendering_map[i][j].size, Rendering_map[i][j].size));//方块大小
        square.setFillColor(Rendering_map[i][j].color);//方块颜色
        square.setPosition({j*Proportion,i*Proportion});//方块位置(地图坐标)
        window.draw(square);//绘制方块
        }
    }
    window.display();//显示窗口

    std::stringstream Log_1;
    Log_1 << "Progress." << mapdata;
    Log_output(Log_1.str(),Log_Information);
}

void Maze_AI::Render_cout(int y,int x){
switch(maze_map[y][x]){
    case Maze_empty_Enum://空
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_empty_colors.r, Maze_empty_colors.g, Maze_empty_colors.b)};break;
    case Maze_walls_Enum:
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_walls_colors.r, Maze_walls_colors.g, Maze_walls_colors.b)};break;
    case Maze_Destination://red
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_Destination_colors.r, Maze_Destination_colors.g, Maze_Destination_colors.b)};break;
    case Maze_Starting_point://Blue
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_Starting_point_colors.r, Maze_Starting_point_colors.g, Maze_Starting_point_colors.b)};break;
    case Maze_aipath_Enum://Green
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_aipath_colors.r, Maze_aipath_colors.g, Maze_aipath_colors.b)};break;
    case Maze_Shortest_route://Magenta
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_Shortest_route_colors.r, Maze_Shortest_route_colors.g, Maze_Shortest_route_colors.b)};break;
    case Maze_Searching_Traces1:
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_Searching_Traces1_colors.r, Maze_Searching_Traces1_colors.g, Maze_Searching_Traces1_colors.b)};break;
    case Maze_Searching_Traces2:
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_Searching_Traces2_colors.r, Maze_Searching_Traces2_colors.g, Maze_Searching_Traces2_colors.b)};break;
    case Gold_Coin_Special:
    Rendering_map[y][x] = {Proportion,sf::Color(Gold_Coin_Special_colors.r, Gold_Coin_Special_colors.g, Gold_Coin_Special_colors.b)};break;
    case Scan_Mark:
    Rendering_map[y][x] = {Proportion,sf::Color(Scan_Mark_colors.r, Scan_Mark_colors.g, Scan_Mark_colors.b)};break;
    case Gold_Coin_Tag:
    Rendering_map[y][x] = {Proportion,sf::Color(Gold_Coin_Tag_colors.r, Gold_Coin_Tag_colors.g, Gold_Coin_Tag_colors.b)};break;
    default://未知数据
    Rendering_map[y][x] = {Proportion,sf::Color(Maze_unknown_colors.r, Maze_unknown_colors.g, Maze_unknown_colors.b)};break;
    } 
}



void Maze_AI::Algorithm_selection(int Create,int Search){//算法启动以及数据检查
//检查算法库是否存在
if(Create>=Algorithm_Library_Create.size() || Create<0){
    exits("Cannot find the create algorithm library",Error_exit);
}
if(Search>=Algorithm_Library_Search.size() || Search<0){
    exits("Cannot find the search algorithm library",Error_exit);
}
if(Algorithm_Library_Create[Create].Type!=Algorithm_Library_Search[Search].Type){
    exits("The type of the two algorithms does not match",Error_exit);
}


//检查数据合理性    
if(Ymax<map_Smallest || Xmax<map_Smallest || Ymax>Map_Maximum || Xmax>Map_Maximum){
exits("XY map data error! [Name_-x_-y]",Error_exit);
}else if(Ymax >= Adjust_Zoom || Xmax >= Adjust_Zoom){//调整缩放比例
    int Ymax_Proportion = Ymax , Xmax_Proportion = Xmax;
    while(Ymax_Proportion >= Adjust_Zoom || Xmax_Proportion >= Adjust_Zoom){
    Proportion = Proportion/2;//调整缩放比例
    Ymax_Proportion = Ymax_Proportion/2;
    Xmax_Proportion = Xmax_Proportion/2;
    }
    Log_stream << "Adjusted proportion[" << Proportion << "]";
    Log_output(Log_stream.str(),Log_Warning);
}


//记录选择的算法(结算数据时使用)
Select_Create = Create;
Select_Search = Search;
Start_Time = Get_the_time();//时间初始化
//初始化数据

maze_map.assign(Ymax, std::vector<int>(Xmax,Algorithm_Library_Create[Create].Initialization));  //地图初始值0
Rendering_map.assign(Ymax, std::vector<CircleData>(Xmax));//渲染数据缓存

Log_output("====== Initialized the map data ======",Log_Information);

    //窗口启动
    std::stringstream Current_content,Current_content_1,Current_content_2;
    int Window_width = Xmax*Proportion;
    int Window_height = Ymax*Proportion;
    Current_content_1 << Algorithm_Library_Create[Select_Create].Name << "and" << Algorithm_Library_Search[Select_Search].Name;
    window = sf::RenderWindow(sf::VideoMode({static_cast<unsigned int>(Window_width),static_cast<unsigned int>(Window_height)}), Current_content_1.str());

    Current_content << "Start generating:" << Algorithm_Library_Create[Create].Name ;
    Log_output(Current_content.str(),Log_Information);

    Algorithm_Library_Create[Create].Run();//生成
    End_Point();//终起xy位置初始化

    Current_content_2 << "Start searching:" << Algorithm_Library_Search[Search].Name;
    Log_output(Current_content_2.str(),Log_Information);

    Algorithm_Library_Search[Search].Run();//搜索
    map_Render_Display();//最后一次刷新地图

    Settlement();//结算数据
    //window.close();//关闭窗口
    Close_window();
}

//工具函数实现

bool Maze_AI::Boundary_check(int x,int y){//边界检查
    Log_stream << "Boundary_check[" << x << "," << y << "]:" << (x < Xmax && x >= 0 && y < Ymax && y >= 0);
    Log_output(Log_stream.str(),Log_Information);
    return (x < Xmax && x >= 0 && y < Ymax && y >= 0);
}

bool Maze_AI::Check_index(int Index){
    Log_stream << "Index_check[" << Index << "]:" << (Index >= 0) && (Index + 1 <= Xmax * Ymax);
    Log_output(Log_stream.str(),Log_Information);
    return (Index >= 0) && (Index + 1 <= Xmax * Ymax);
}

void Maze_AI::Mark_Trace(int x,int y,int Type){//快捷标记
    if(Boundary_check(x,y)){
    maze_map[y][x]=Type;
    Log_stream << "Changed[" << x << "," << y << "]:" << Type;
    Log_output(Log_stream.str(),Log_Information);   
    }else{
    Log_stream << "Setting failed[" << x << "," << y << "]:" << Type;
    Log_output(Log_stream.str(),Log_Warning);//设置失败
    exits("Exceeding map boundaries when setting a marker",Error_exit);
    }

}
int Maze_AI::Location_Marker(int x,int y){//返回位置标记
    if(Boundary_check(x,y)){
    Log_stream << "Get data[" << x << "," << y << "]:" << maze_map[y][x];
    Log_output(Log_stream.str(),Log_Information);
    return maze_map[y][x];
    }else{
        Log_stream << "Failed to obtain[" << x << "," << y << "]:" << "null";
        Log_output(Log_stream.str(),Log_Warning);
        exits("The acquired location is beyond the map");
        return 0;
    }
}
bool Maze_AI::Tag_Information(int x,int y,int Type){//检查特定标记
    if(!Boundary_check(x,y)){
    Log_stream << "Data check failed[" << x << "," << y << "]:" << Type;
    Log_output(Log_stream.str(),Log_Warning);
    exits("Exceeds map range when checking the mark",Error_exit);
    }
    Log_stream << "Check location data[" << x << "," << y << "]:" << (maze_map[y][x]==Type);
    Log_output(Log_stream.str(),Log_Information);
    return (maze_map[y][x]==Type);
}
int Maze_AI::X_Map_Max(){
Log_stream << "Get data[" <<"Xmax:" << Xmax << "]";
Log_output(Log_stream.str(),Log_Information);
return Xmax;
}
int Maze_AI::Y_Map_Max(){
Log_stream << "Get data[" <<"Ymax:" << Ymax << "]";
Log_output(Log_stream.str(),Log_Information);
return Ymax;
}
void Maze_AI::Set_destination(int Index){
if(Check_index(Index)){
Log_stream << "Set destination[" <<"Index:" << Index << "]";
Log_output(Log_stream.str(),Log_Information);
Destination=Index;
    }else{
Log_stream << "Failed to set destination[" <<"Index:" << Index << "]";
Log_output(Log_stream.str(),Log_Warning);
exits("Exceeding the map boundaries when setting the destination",Error_exit);
    }
}
void Maze_AI::Set_Starting_point(int Index){
if(Check_index(Index)){
Log_stream << "Set starting point[" <<"Index:" << Index << "]";
Log_output(Log_stream.str(),Log_Information);
Starting_point=Index;
    }else{
Log_stream << "Failed to set starting point[" <<"Index:" << Index << "]";
Log_output(Log_stream.str(),Log_Warning);
exits("Exceeding the map boundaries when setting the starting point",Error_exit);
    }
}
int Maze_AI::Obtain_destination(){
Log_stream << "SGet data(destination)[" <<"Index:" << Destination << "]";
Log_output(Log_stream.str(),Log_Information);
    return Destination;
}
int Maze_AI::Obtain_Starting_point(){
Log_stream << "Get data(starting point)[" <<"Index:" << Starting_point << "]";
Log_output(Log_stream.str(),Log_Information);
    return Starting_point;
}
bool Maze_AI::Check_Destination(int x,int y){
Log_stream << "Check position endpoint[" << x << "," << y << "]:" << (Destination_X==x && Destination_Y==y);
Log_output(Log_stream.str(),Log_Information);
return (Destination_X==x && Destination_Y==y);
}
bool Maze_AI::Check_starting_point(int x,int y){
Log_stream << "Check starting position[" << x << "," << y << "]:" << (Starting_point_X==x && Starting_point_Y==y);
Log_output(Log_stream.str(),Log_Information);
return (Starting_point_X==x && Starting_point_Y==y);
}
int Maze_AI::Get_indexX(int Index){
if(Check_index(Index)){
Log_stream << "Get data(Index:X)[" << Index << "]:" << Index%Xmax;
Log_output(Log_stream.str(),Log_Information);
    return Index%Xmax;
}else{
    Log_stream << "Failed to obtain(Index:X)[" << Index << "]:" << Index%Xmax;
    Log_output(Log_stream.str(),Log_Warning);
    exits("Exceeding the map boundaries when obtaining the indexX",Error_exit);
    return 0;
}
}
int Maze_AI::Get_indexY(int Index){
if(Check_index(Index)){
Log_stream << "Get data(Index:Y)[" << Index << "]:" << Index/Xmax;
Log_output(Log_stream.str(),Log_Information);
    return Index/Xmax;
}else{
    Log_stream << "Failed to obtain(Index:Y)[" << Index << "]:" << Index/Xmax;
    Log_output(Log_stream.str(),Log_Warning);
    exits("Exceeding the map boundaries when obtaining the indexY",Error_exit);
    return 0;
}
}

int Maze_AI::Random_number(int Min,int Max){
if (Min > Max){
    std::swap(Min, Max);
    Log_stream << "Swapped Min and Max[" << Min << "," << Max << "]";
    Log_output(Log_stream.str(),Log_Warning);
}
static std::random_device rd;// 获取真随机数种子
static std::mt19937 rng(rd());//创建随机数引擎
std::uniform_int_distribution<int> dist(Min,Max);
return dist(rng);
}

int Maze_AI::Get_the_time(){//获取当前时间
auto Start_Time_void = std::chrono::duration_cast<std::chrono::seconds>(
std::chrono::system_clock::now().time_since_epoch()
);

return Start_Time_void.count();//返回
}

//收尾函数实现

void Maze_AI::Close_window(){//关闭窗口(延迟)
//获取当前时间
int End_time = Get_the_time()+End_waiting;

//输出日志
Log_stream << "Waiting for the end of the program[" << End_waiting << "s]";
Log_output(Log_stream.str(),Log_Information);

while(Get_the_time()<End_time){//等待关闭窗口
Processing_window();//处理窗口状态
Interactive_State_window();//维护交互状态
sf::sleep(sf::milliseconds(10));//刷新间隔
}
window.close();
}


void Maze_AI::map_Render_Display(){//刷新起点终点
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

void Maze_AI::Log_output(std::string message,Log_Type Type){//日志输出
    std::cout << "\033[2m[" <<Time_Duration<<"s]\033[0m" ;//输出时间

    switch (Type)// 输出日志类型
    {
    case Log_Error:
        std::cout << "\033[31m[!]\033[0m" << message;break;
        case Log_Warning:
        std::cout << "\033[33m[*]\033[0m" << message;break;
        case Log_Information:
        std::cout << "\033[32m[i]\033[0m" << message;break;
        default://未知类型
        std::cout << "\033[32m[?]\033[0m" << message;break; 
    }
    std::cout << std::endl;//换行
    Log_stream.str("");
    Log_stream.clear();
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
std::cout << "\033[31m[?]\033[0mUnknown error.-->\033[4m"<< Error_message << "\033[0m";
    break;
}
exit(0);//程序退出
}
//其他函数

void Maze_AI::cout_title(){
std::cout << "      "<<"_______"<< "  "<<"______"<<"   "<<"______________"<<std::endl;
std::cout << "     "<<"/  ___  \\"<<" "<<"\\____/"<<" "<<"/  ___   ___   /"<<std::endl;
std::cout << "    "<<"/  /   |  |"<< ""<<" ___  "<<""<<"/  /  /  /  /  /"<<std::endl;
std::cout << "   "<<"/  /   /  /"<< ""<<" /  / "<<""<<"/  /  /  /  /  /"<<std::endl;
std::cout << "  "<<"/  /   /  /"<< ""<<" /  / "<<""<<"/  /  /  /  /  /"<<std::endl;
std::cout << " "<<"/  /__ /  /"<< ""<<" /  / "<<""<<"/  /  /  /  /  /"<<std::endl;
std::cout << ""<<"/_________/"<<""<<" /__/ "<<""<<"/__/  /__/  /__/"<<std::endl;
//std::cout << "Maze_AI_Project_start_time[20260427]"<<std::endl;
std::cout << "Maze_AI_AI Version:"<< Project_Version <<std::endl;
std::cout << std::endl;
}
