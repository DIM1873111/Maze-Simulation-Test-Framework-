#include <iostream>
#include <vector>
#include <string>
#include <limits>  // numeric_limits
#include <sstream>
#include <memory>
#include <algorithm>  // shuffle
#include <random>
#include <functional>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "Maze2.h"//Maze2.h头文件

//第三方库
#include "third_party\kissnet.hpp"
#include "third_party\json.hpp"
#include "third_party\process\process.hpp"



//__________________________________Config_Reading__________________________________


void Mazesimulate::Config_Reading::Add_config_table(){
    config = {
        {"Map_length_X", &mazesimulate->Map_length_X, Data_type::INT_ENUM},
        {"Map_length_Y", &mazesimulate->Map_length_Y, Data_type::INT_ENUM},
        {"Map_Max_area", &mazesimulate->Map_Max_area, Data_type::INT_ENUM},
        {"Map_mmin_x", &mazesimulate->Map_Min_X, Data_type::INT_ENUM},
        {"Map_mmin_y", &mazesimulate->Map_Min_Y, Data_type::INT_ENUM},
        {"Max_number_logs", &mazesimulate->Max_number_logs, Data_type::INT_ENUM},
        {"Render_Port_a", &mazesimulate->Render_Port_A, Data_type::INT_ENUM},
        {"Render_Port_b", &mazesimulate->Render_Port_B, Data_type::INT_ENUM},
        {"Calculation_interval", &mazesimulate->Calculation_interval, Data_type::INT_ENUM},
        {"Render_IP", &mazesimulate->Render_IP, Data_type::STRING_ENUM}
    };
}



void Mazesimulate::Config_Reading::Load_Config(){
    try{
        if(Check_file(configPath)){//如果配置文件存在
            Update_config(config, Open_json_file(configPath));//更新配置信息
        }else{//如果配置文件不存在
            Write_file(configPath, Create_json_through(config));//创建配置文件并写入默认配置
        }
    }catch(const std::runtime_error& e){//捕获异常
        mazesimulate->Log_class.cout_Log_system(e.what(), Log_Exit::Log_type::WARNING_ENUM);//输出警告日志
    }catch(...){
        mazesimulate->Log_class.cout_Log_system("Unknown error occurred", Log_Exit::Log_type::ERROR_ENUM);//输出错误日志
    }

}

void Mazesimulate::Config_Reading::Create_Config(){//创建json文件 并且写入默认配置

    try{
        Clean_files(configPath);//清理配置文件
        Write_file(configPath, Create_json_through(config));//创建json文件并写入默认配置
    }catch(const std::runtime_error& e){//捕获异常
        mazesimulate->Log_class.cout_Log_system(e.what(), Log_Exit::Log_type::WARNING_ENUM);//输出警告日志
    }catch(...){
        mazesimulate->Log_class.cout_Log_system("Unknown error occurred", Log_Exit::Log_type::ERROR_ENUM);//输出错误日志
    }

}



//__________________________________Log_Exit__________________________________



void Mazesimulate::Log_Exit::cout_Log_system(std::string log, Log_type type){//输出日志信息并记录日志信息

std::stringstream Temporary_log;//临时存储日志信息

switch(type){

    case Log_type::PROMPT_ENUM://提示日志
        Temporary_log << "[\033[1mPROMPT\033[0m]" << log << std::endl;
    break;

    case Log_type::NOTICE_ENUM://通知日志
        Temporary_log << "[\033[34mNOTICE\033[0m]" << log << std::endl;
    break;

    case Log_type::WARNING_ENUM://警告日志
        Temporary_log << "[\033[33mWARNING\033[0m]" << log << std::endl;
    break;

    case Log_type::ERROR_ENUM://错误日志
        Temporary_log << "[\033[31mERROR\033[0m]" << log << std::endl;
    break;

}
//NOTICE
std::cout << Temporary_log.str();//输出日志信息
//记录日志信息(queue)

/*

*/
}



void Mazesimulate::Log_Exit::record_Log_Algorithm(std::string log, Algorithm_Log type){//记录算法日志信息

std::stringstream Temporary_log;//临时存储日志信息

switch(type){

    case Algorithm_Log::NORMAL_ENUM://通知日志
        Temporary_log << "[\033[34mALGORITHM_NORMAL\033[0m]" << log << std::endl;
    break;

    case Algorithm_Log::ERROR_ENUM://错误日志
        Temporary_log << "[\033[31mALGORITHM_ERROR\033[0m]" << log << std::endl;
    break;

    case Algorithm_Log::PROCESS_ENUM://处理日志(算法处理流程)
        Temporary_log << "[\033[33mALGORITHM_PROCESS\033[0m]" << log << std::endl;
    break;

    case Algorithm_Log::INITIAL_ENUM://初始化日志(数据初始化)
        Temporary_log << "[\033[33mALGORITHM_INITIAL\033[0m]" << log << std::endl;
    break;

}


historical_logs.push(Temporary_log.str());

if(historical_logs.size() > (size_t)mazesimulate->Max_number_logs){//如果日志容器大小超过最大日志数

    historical_logs.pop();//删除最早的日志信息

}

}

void Mazesimulate::Log_Exit::Exit(std::string exit_log, Exit_type type){//输出错误信息并退出程序

    std::stringstream Logout_Information;//临时存储错误信息


switch(type){

    case Exit_type::STANDARD_ENUM://标准退出
        Logout_Information << "[\033[2mEXIT\033[0m]" << exit_log << std::endl;
        break;

    case Exit_type::OPERATIONAL_ENUM://退出程序
        Logout_Information << "[\033[33mEXIT_OPERATIONAL\033[0m]" << exit_log << std::endl;
        break;

    case Exit_type::SYSTEM_ENUM://系统退出
        Logout_Information << "[\033[90mEXIT_SYSTEM\033[0m]" << exit_log << std::endl;
        break;
    
}




std::cout << Logout_Information.str();//输出错误信息

exit(0);//退出程序

}


void Mazesimulate::Log_Exit::Export_history_log(){

        //输出退出前的错误信息
        //如果日志容器为空跳过
    int logsize = historical_logs.size();
    std::cout << "[\033[38;5;214mERROR_LOG\033[0m]Output the last'" << historical_logs.size() << "'logs before exit" << std::endl; //输出最后的错误日志
    if(historical_logs.empty()){
        
        
    }else{

    while(!historical_logs.empty()){
        std::cout << historical_logs.front();
        historical_logs.pop();
        }        
    }

std::cout << "[The end]" << "Outputted " << logsize << "/" << mazesimulate->Max_number_logs << " logs and the container has been cleared" << std::endl;

}


//__________________________________Initialize__________________________________



void Mazesimulate::Input_processing::Create_table(){//创建指令表
Parameter_table.push_back({"-help","Help with operations", [this](){help_operate();}});//添加默认参数
Parameter_table.push_back({"-g","Start the simulation",[](){}});//添加默认开始参数
Parameter_table.push_back({"-initmap","Use current data Initialize the map", [this](){mazesimulate->Map_class.Initialize_map_data();}});
Parameter_table.push_back({"-exit","Exit the simulation", [this](){mazesimulate->Log_class.Exit("Simulation exited", Log_Exit::Exit_type::OPERATIONAL_ENUM);}});//添加默认退出参数
Parameter_table.push_back({"-resetjson","Reset the json file", [this](){Resetjson_operation();}});//添加默认重置json参数
Parameter_table.push_back({"-listmethods","List the algorithms", [this](){List_algorithms();}});
Parameter_table.push_back({"-make","Run the generation algorithm", [this](){mazesimulate->Algorithm_class.Run_make_algorithm(mazesimulate->Selected_generation_method);}});
Parameter_table.push_back({"-find","Run a search algorithm", [this](){mazesimulate->Algorithm_class.Run_find_algorithm(mazesimulate->Selected_search_method);}});
Parameter_table.push_back({"-methodlog","List the method log and clear the container", [this](){mazesimulate->Log_class.Export_history_log();}});
Parameter_table.push_back({"-mapinfo", "Check map information", [this](){Check_mapinfo();}});
Parameter_table.push_back({"-version","Check version info", [this](){Version_Info();}});

config = {
    {"mapx", "Map X length", &mazesimulate->Map_length_X, Data_type::INT_ENUM},
    {"mapy", "Map Y length", &mazesimulate->Map_length_Y, Data_type::INT_ENUM},
    {"mapmax","Map Max 'area'", &mazesimulate->Map_Max_area, Data_type::INT_ENUM},
    {"mapmminx", "Map Min X length", &mazesimulate->Map_Min_X, Data_type::INT_ENUM},
    {"mapmminy", "Map Min Y length", &mazesimulate->Map_Min_Y, Data_type::INT_ENUM},
    {"make","Choose the number of the generation method", &mazesimulate->Selected_generation_method, Data_type::INT_ENUM},
    {"find","Choose the number of the search method", &mazesimulate->Selected_search_method, Data_type::INT_ENUM},
    {"porta", "Graphical rendering port", &mazesimulate->Render_Port_A, Data_type::INT_ENUM},
    {"portb", "Graphical rendering port", &mazesimulate->Render_Port_B, Data_type::INT_ENUM},
    {"interval", "Calculation interval", &mazesimulate->Calculation_interval, Data_type::INT_ENUM},
    {"Choose_make","Chosen generation algorithm", &mazesimulate->Selected_generation_method, Data_type::INT_ENUM},
    {"Choose_find","Chosen search algorithm", &mazesimulate->Selected_search_method, Data_type::INT_ENUM},
    {"render_ip","Graphical rendering IP", &mazesimulate->Render_IP, Data_type::STRING_ENUM},
};

}




void Mazesimulate::Input_processing::help_operate(){//帮助操作
std::cout << "Available parameters(Default action function):" << std::endl;
if(config.empty()){

    std::cout << "No default action registered." << std::endl;
    
}else{

  for(const auto& Parameter : Parameter_table){
    std::cout << "[\033[90m" << Parameter.name << "\033[0m]" << Parameter.Guide << std::endl;
    }
    std::cout << "[\033[90m(CTRL+C)\033[0m]" << "Exit the simulation(-EOF)" << std::endl;
}
//配置列表
std::cout << "Config Parameter Table:" << std::endl;
if(config.empty()){

    std::cout << "No configuration parameters available." << std::endl;

}else{

    for(const auto& config : config){
    std::string config_name = variantToString(ToValueVariant(config.Address));//转换配置名
    std::cout << "[\033[90m" << config.config_name << "\033[0m]" << config.config_description << "\033[2m\033[5m\033[3m --> \033[0m{[" << config_name << "]" ;
    std::cout << Type_Name(config.data_type) << "}" << std::endl;
    }
    

}
std::cout << "Parameter Usage:" << std::endl;
std::cout << "Just use 'parameterName' + ' " << delimiter <<" ' + 'parameter'" << std::endl;
std::cout << "Directly use the function '-Function_Name' " << std::endl;
std::cout << "You need a space between every parameter or function" << std::endl;
std::cout << "Execute from left to right" << std::endl;
}


void Mazesimulate::Input_processing::Check_mapinfo(){//检查地图信息
Mazesimulate::Map_data::Map_Info map_info = mazesimulate->Map_class.get_map_info();//获取地图信息
std::cout << "Map Status:";
switch (map_info.map_status){
    case Mazesimulate::Map_data::Map_Status::NOT_INITIALIZED_ENUM://未初始化
        std::cout << "\033[2mNot initialized\033[0m" << std::endl;
        break;

    case Mazesimulate::Map_data::Map_Status::INITIALIZED_ENUM://初始化
        std::cout << "Initialized" << std::endl;
    break;

    case Mazesimulate::Map_data::Map_Status::GENERATED_ENUM://生成
        std::cout << "\033[32m\033[2mGenerated\033[0m" << std::endl;
    break;

}
std::cout << "Map Length X:" << map_info.practical_X << std::endl;
std::cout << "Map Length Y:" << map_info.practical_Y << std::endl;
std::cout << "Map Area:" << map_info.map_area << std::endl;

}


void Mazesimulate::Input_processing::Resetjson_operation(){//重置json文件

mazesimulate->Log_class.cout_Log_system("Reset Config File", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志
mazesimulate->Config_class.Create_Config();//创建json文件 并且写入默认配置
mazesimulate->Log_class.cout_Log_system("Operation completed", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志

}

void Mazesimulate::Input_processing::Version_Info(){
std::string Topic = R"(
 ____ ____ _________ _________ _________
|    |    |         |         |         |
|         |    |    |_____    |    _____|
|  |   |  |    |    |         |         |
|  |   |  |         |    _____|    _____|
|  |   |  |    |    |         |         |
|__|___|__|____|____|_________|_________|
[Maze2.Frame|Simulation|Terminal]
Github:https://github.com/DIM1873111/Maze2
Email:outlook_F1DEC00551A56B19@outlook.com
Current version: 0.4.3[Beta version]
)";

std::cout << Topic << std::endl;

}

void Mazesimulate::Input_processing::List_algorithms(){


    std::cout << "find algorithm list:" << std::endl;
    if(mazesimulate->Algorithm_class.find_algorithm_Info.empty()){
        std::cout << "[\033[90mNo find algorithm available.\033[0m]" << std::endl;
    }else{
        for(int i = 0; i < (int)mazesimulate->Algorithm_class.find_algorithm_Info.size(); i++){

            std::cout << "[\033[90m" << i << " -> " << mazesimulate->Algorithm_class.find_algorithm_Info[i].name << " \033[0m] " << mazesimulate->Algorithm_class.find_algorithm_Info[i].description << std::endl;

        }   
    }

    std::cout << "make algorithm list:" << std::endl;

    if(mazesimulate->Algorithm_class.make_algorithm_Info.empty()){
        std::cout << "[\033[90mNo make algorithm available.\033[0m]" << std::endl;
    }else{
        for(int i = 0; i < (int)mazesimulate->Algorithm_class.make_algorithm_Info.size(); i++){

            std::cout << "[\033[90m" << i << " -> " << mazesimulate->Algorithm_class.make_algorithm_Info[i].name << " \033[0m] " << mazesimulate->Algorithm_class.make_algorithm_Info[i].description << std::endl;

        }        
    }

}


Mazesimulate::Input_processing::Go_back Mazesimulate::Input_processing::Matching_Parameter(std::string Input){//匹配参数

Go_back Back_data = {false, 0};//设置默认参数为未找到

for(int i = 0; (size_t)i < Parameter_table.size(); i++){

if(Parameter_table[i].name == Input){//如果输入参数与配置表中的参数匹配
Back_data = {true, i};//设置匹配参数为匹配
break;//跳出循环
}

}

return Back_data;//返回匹配的参数
}



void Mazesimulate::Input_processing::Processing_input(std::string Input){


std::vector<std::string> Input_vec = Transform_str_vec(Input);//解析输入数据

for(const auto& Currently : Input_vec){

Go_back Back_data = Matching_Parameter(Currently);//匹配参数

    if(Back_data.Find_the_mood){//判断输入参数是否找到

        try{
            Parameter_table[Back_data.Location].Function();//调用配置表中的动作函数
        }catch(const std::runtime_error& e){
            mazesimulate->Log_class.cout_Log_system(Parameter_table[Back_data.Location].name + e.what(), Log_Exit::Log_type::WARNING_ENUM);//输出警告日志
        }catch(...){
            mazesimulate->Log_class.cout_Log_system("Unknown type error", Log_Exit::Log_type::ERROR_ENUM);//输出警告日志
        }


    }else{//未找到

        Matching_config_table(splittwo(Currently, delimiter));//匹配配置表

    }

    }

}



void Mazesimulate::Input_processing::Get_Input(){

    std::vector<std::string> inputs; //临时存储输入的容器
    std::string token; //临时存储每个字符串

    std::cout << "Enter parameters to start the simulation" << std::endl;
    std::cout << "[type '-help' to see available parameters]" << std::endl;
    std::cout << "> ";


    while(std::getline(std::cin, token)){//读取输入
        try{

            Processing_input(token);//处理输入数据(入口)

        }catch(const std::runtime_error& e){
            std::string errorStr = e.what();
            mazesimulate->Log_class.cout_Log_system(e.what(), Log_Exit::Log_type::WARNING_ENUM);//输出警告日志
        
        }
            

       std::cout << "> " ;
    }

    std::cout << "'-EOF'" << std::endl;
    mazesimulate->Log_class.Exit("Input EOF to exit", Log_Exit::Exit_type::OPERATIONAL_ENUM);//跳出程序

}


void Mazesimulate::Input_processing::Matching_config_table(std::pair<std::string, std::string> Input){//匹配配置表

for(const auto& configs : config){
    
    if(Input.first == configs.config_name){//pair
        Data_type type = analyzing_type(Input.second);//分析输入参数类型
        if(type == configs.data_type){
            InputType_Conversion(Input.second, configs.Address, type);//转换配置值
        }else{
            mazesimulate->Log_class.cout_Log_system("Type error The actual type is:" + Type_Name(type), Log_Exit::Log_type::WARNING_ENUM);
        }

        break;
    }
}

}

//__________________________________Data_Processing__________________________________

void Mazesimulate::Data_Processing::Initialize_data(){//初始化数据



}

int Mazesimulate::Data_Processing::Stats_route_data(Mazesimulate::Map_data::Map_type map_type){//统计路线数据
int Temporary_statistics = 0;//临时统计路线数据
    
for(int i = 0; i < mazesimulate->Map_length_Y; i++){
        for(int j = 0; j < mazesimulate->Map_length_X; j++){
            if(mazesimulate->Map_class.Check_a_tag(j,i, map_type)){
                Temporary_statistics++;//统计路线格数量
            }
        }
    }


    return Temporary_statistics;//返回统计路线数据

}


//PS:统计数据前指的是在调用搜索算法之前使用的统计数据函数 也就是刚刚运行完创造算法法之后 统计数据后同理


void Mazesimulate::Data_Processing::Statistics_Before(){//统计数据前

    Mazesimulate::Map_data::Map_Info map_info = mazesimulate->Map_class.get_map_info();//获取地图信息

    Map_route = Stats_route_data(Map_data::Map_type::EMPTY_ENUM);//统计路线格数量

    Map_route_proportion = (double)Map_route / (map_info.map_area) * 100;//计算路线格比例

    std::stringstream Temporary_log;
    Temporary_log << "Map area " << map_info.map_area << "(" << map_info.practical_X << "*" << map_info.practical_Y << ")" << "Among them, " << Map_route_proportion << "%(" << Map_route << ")" << " are all";
    mazesimulate->Log_class.cout_Log_system(Temporary_log.str(), Log_Exit::Log_type::PROMPT_ENUM);//输出提示日志
}

void Mazesimulate::Data_Processing::Statistics_After(){//统计数据后

    //Mazesimulate::Map_data::Map_Info map_info = mazesimulate->Map_class.get_map_info();//获取地图信息

    int SearchMap_route = Stats_route_data(Map_data::Map_type::EMPTY_ENUM);//统计路线格数量

    int Search = Map_route - SearchMap_route;//计算搜索算法后路线格数量

    int Search_map = (double)Search / Map_route * 100;//计算搜索算法后路线格比例


    std::stringstream Temporary_log;
    Temporary_log << "explored " << Search_map << "%(" << Search << ") Map route";
    mazesimulate->Log_class.cout_Log_system(Temporary_log.str(), Log_Exit::Log_type::PROMPT_ENUM);//输出提示日志


}






//__________________________________Mazesimulate__________________________________


void Mazesimulate::Framework_Entry(){//初始化


Data_class.Initialize_data();//初始化数据
Input_class.Get_Input();//调用用户输入

}


void Mazesimulate::add_make_Method(std::string name, std::string description, std::function<void(Mazesimulate&)> Function_address){
Algorithm_class.Add_make_algorithm(name, description, Function_address);//添加生成算法
}

void Mazesimulate::add_find_Method(std::string name, std::string description, std::function<void(Mazesimulate&)> Function_address){
Algorithm_class.Add_find_algorithm(name, description, Function_address);//添加搜索算法
}

//__________________________________Map_data__________________________________
void Mazesimulate::Map_data::Initialize_map_data(){//初始化地图数据
//获取地图信息
int Create_X = mazesimulate->Map_length_X;//地图长度X
int Create_Y = mazesimulate->Map_length_Y;//地图长度Y
int Create_area = Create_X * Create_Y;//地图面积
int Min_X = mazesimulate->Map_Min_X;//地图最小长度X
int Min_Y = mazesimulate->Map_Min_Y;//地图最小长度Y
int Max_area = mazesimulate->Map_Max_area;//地图最大面积

if(Create_X > std::max(Min_X,5) && Create_Y > std::max(Min_Y,5) && Create_area <= Max_area){

map_data.assign(Create_Y, std::vector<Map_type>(Create_X, Map_type::EMPTY_ENUM));//初始化地图数据

map_info = {Create_X, Create_Y,Create_area, Map_Status::INITIALIZED_ENUM};//初始化地图信息

mazesimulate->Log_class.cout_Log_system("Map initialized:" + std::to_string(map_info.practical_X) + "," + std::to_string(map_info.practical_Y) , Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志
   
}else{


mazesimulate->Log_class.cout_Log_system("The size set is smaller than the min limit - > Cancel Creation.", Log_Exit::Log_type::WARNING_ENUM);//输出警告日志

}

}

void Mazesimulate::Map_data::Set_map_alltype(Map_type type){//设置地图所有位置的标记

if(map_info.map_status == Map_Status::NOT_INITIALIZED_ENUM){//检查地图是否初始化
    throw std::runtime_error("[Set_map_alltype]Map not initialized");//输出错误日志
}

for(auto& row : map_data){
    std::fill(row.begin(), row.end(), type);
}

add_map_status(Map_Status::INITIALIZED_ENUM);//设置地图状态已初始化

mazesimulate->Log_class.cout_Log_system("Map all set to - > " + static_cast<int>(type) , Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志

}



void Mazesimulate::Map_data::add_map_status(Map_Status map_status){
map_info.map_status = map_status;//设置地图状态
}

Mazesimulate::Map_data::Map_Info Mazesimulate::Map_data::get_map_info(){//获取地图信息

return map_info;//返回地图信息

}


std::vector<std::vector<Mazesimulate::Map_data::Map_type>> Mazesimulate::Map_data::Get_map_data(){//获取地图数据

return map_data;//返回地图数据

}



bool Mazesimulate::Map_data::Check_location_range(int x, int y){//检查坐标是否在地图范围内
if(map_info.map_status == Map_Status::NOT_INITIALIZED_ENUM){//检查地图是否初始化
    throw std::runtime_error("[Check_location_range]Map not initialized");
}
return (x >= 0 && x < map_info.practical_X) && (y >= 0 && y < map_info.practical_Y);

}

bool Mazesimulate::Map_data::Check_a_tag(int x, int y , Map_type a_tag){
if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
throw std::runtime_error("[Check_a_tag]Accessing the map beyond its size");
}
return map_data[y][x] == a_tag;//检查坐标是否包含指定的标记
}

bool Mazesimulate::Map_data::Check_the_finish(int x, int y){
if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
throw std::runtime_error("[Check_the_finish]Accessing the map beyond its size");
}

return x == finish_position.x && y == finish_position.y;//;//检查坐标是否包含指定的标记

}

bool Mazesimulate::Map_data::Check_the_start(int x, int y){
if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
throw std::runtime_error("[Check_the_start]Accessing the map beyond its size");
}

return x == start_position.x && y == start_position.y;//检查坐标是否包含指定的标记

}

Mazesimulate::Map_data::Map_type Mazesimulate::Map_data::Get_location_tag(int x, int y){

    if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
        throw std::runtime_error("[Get_location_tag]Accessing the map beyond its size");//输出错误日志
    }

    return map_data[y][x];//返回地图位置标签数据

}

void Mazesimulate::Map_data::Set_location_tag(int x, int y, Map_type a_tag){//设置地图位置标签数据
    if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
        throw std::runtime_error("[Set_location_tag]Accessing the map beyond its size");//输出错误日志
    }

    map_data[y][x] = a_tag;//设置地图位置标签数据

}

void Mazesimulate::Map_data::Set_finish_position(int x, int y){//设置终点位置

    if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
        throw std::runtime_error("[Set_finish_position]Accessing the map beyond its size");//输出错误日志
    }

finish_position.x = x;
finish_position.y = y;
}

void Mazesimulate::Map_data::Set_start_position(int x, int y){//设置起点位置

    if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
        throw std::runtime_error("[Set_start_position]Accessing the map beyond its size");//输出错误日志
    }

start_position.x = x;
start_position.y = y;
}


Mazesimulate::Map_data::Location Mazesimulate::Map_data::Get_finish_position(){//获取终点位置
    return finish_position;//返回终点位置
}

Mazesimulate::Map_data::Location Mazesimulate::Map_data::Get_start_position(){//获取起点位置
    return start_position;//返回起点位置
}

Mazesimulate::Map_data::Location Mazesimulate::Map_data::Get_mapsize(){
    return Location{map_info.practical_X, map_info.practical_Y};//返回地图大小
}


//___________________________________Algorithm_Tools___________________________________



void Mazesimulate::Algorithm_Tools::Add_make_algorithm(std::string name, std::string description, std::function<void(Mazesimulate&)> Function_address){

    make_algorithm.push_back(Function_address);
    make_algorithm_Info.push_back({name, description});

}



void Mazesimulate::Algorithm_Tools::Add_find_algorithm(std::string name, std::string description, std::function<void(Mazesimulate&)> Function_address){

    find_algorithm.push_back(Function_address);
    find_algorithm_Info.push_back({name, description});

}


void Mazesimulate::Algorithm_Tools::Run_make_algorithm(int index){

    if(index < 0 || index >= (int)make_algorithm.size()){
        throw std::runtime_error("[Run_make_algorithm]Index out of range");
    }

    Mazesimulate::Map_data::Map_Info map_info = mazesimulate->Map_class.get_map_info();//获取地图信息

    if(map_info.map_status != Map_data::Map_Status::INITIALIZED_ENUM){
        throw std::runtime_error("[Run_make_algorithm]Map not initialized");//输出错误日志
    }

    try{

    make_algorithm[index](*mazesimulate);//运行生成算法

    mazesimulate->Map_class.add_map_status(Map_data::Map_Status::GENERATED_ENUM);//设置地图状态已生成

    mazesimulate->Data_class.Statistics_Before();//统计数据前

    }catch(const std::runtime_error& e){
        std::string errorStr = e.what();//捕获异常转化为字符串
        mazesimulate->Log_class.cout_Log_system(e.what(), Log_Exit::Log_type::ERROR_ENUM);//输出错误日志
        //输出日志容器
        mazesimulate->Log_class.Export_history_log();//输出日志容器
    }


}

void Mazesimulate::Algorithm_Tools::Run_find_algorithm(int index){

    if(index < 0 || index >= (int)find_algorithm.size()){
        throw std::runtime_error("[Run_find_algorithm]Index out of range");
    }

    Mazesimulate::Map_data::Map_Info map_info = mazesimulate->Map_class.get_map_info();

    if(map_info.map_status != Map_data::Map_Status::GENERATED_ENUM){
        throw std::runtime_error("[Run_find_algorithm]Map not generated");//输出错误日志
    }

    try{

    find_algorithm[index](*mazesimulate);//运行搜索算法

    mazesimulate->Data_class.Statistics_After();//统计数据后


    }catch(const std::runtime_error& e){
        std::string errorStr = e.what();//捕获异常转化为字符串
        mazesimulate->Log_class.cout_Log_system(e.what(), Log_Exit::Log_type::ERROR_ENUM);//输出错误日志
        //输出日志容器
        mazesimulate->Log_class.Export_history_log();//输出日志容器
    }


}

//___________________________________map_api___________________________________

bool Mazesimulate::map_api::Check_mapscope(int x, int y){//检查xy范围
    return mazesimulate->Map_class.Check_location_range(x, y);//调用Map_data类中的函数
}

bool Mazesimulate::map_api::Check_Maptype(int x, int y, Mazesimulate::Map_data::Map_type a_tag){//检查地图类型
    return mazesimulate->Map_class.Check_a_tag(x, y, a_tag);//调用Map_data类中的函数
}

bool Mazesimulate::map_api::Check_Mapfinish(int x, int y){//检查地图是否到达终点
    return mazesimulate->Map_class.Check_the_finish(x, y);//调用Map_data类中的函数
}

bool Mazesimulate::map_api::Check_Mapstart(int x, int y){//检查地图是否到达起点
    return mazesimulate->Map_class.Check_the_start(x, y);//调用Map_data类中的函数
}

Mazesimulate::Map_data::Map_type Mazesimulate::map_api::Get_maptype(int x, int y){//获取地图类型
    return mazesimulate->Map_class.Get_location_tag(x, y);//调用Map_data类中的函数
}

Mazesimulate::Map_data::Location Mazesimulate::map_api::Get_mapfinish(){//获取终点位置
    return mazesimulate->Map_class.Get_finish_position();//调用Map_data类
}

Mazesimulate::Map_data::Location Mazesimulate::map_api::Get_mapstart(){//获取起点位置
    return mazesimulate->Map_class.Get_start_position();//调用Map_data类中的函数
}

Mazesimulate::Map_data::Location Mazesimulate::map_api::Get_mapsize(){//获取地图大小
    return mazesimulate->Map_class.Get_mapsize();//调用Map_data类中的函数
}

void Mazesimulate::map_api::Set_maptype(int x, int y, Mazesimulate::Map_data::Map_type a_tag){//设置地图类型
    mazesimulate->Map_class.Set_location_tag(x, y, a_tag);//调用Map_data类中的函数
}


void Mazesimulate::map_api::Set_mapfinish(int x, int y){//设置终点
    mazesimulate->Map_class.Set_finish_position(x, y);//调用Map_data类中的函数
}


void Mazesimulate::map_api::Set_mapstart(int x, int y){//设置起点
    mazesimulate->Map_class.Set_start_position(x, y);//调用Map_data类中的函数
}

void Mazesimulate::map_api::Set_allmaptype(Mazesimulate::Map_data::Map_type a_tag){//设置地图所有位置的标记
    mazesimulate->Map_class.Set_map_alltype(a_tag);//调用Map_data类中的函数
}

void Mazesimulate::map_api::Output_log(std::string log, Mazesimulate::Log_Exit::Algorithm_Log type){//输出日志
    mazesimulate->Log_class.record_Log_Algorithm(log, type);//调用Log_class类中的函数
}



//___________________________________Render_Send___________________________________


void Mazesimulate::Render_Send::Settings_Render_Password(){
//生成1000-9999的随机数作为密码
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(1000, 9999);
mazesimulate->Render_Password = dis(gen);

}