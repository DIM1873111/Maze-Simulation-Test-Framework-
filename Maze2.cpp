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
#include "json.hpp"//json库




//__________________________________Config_Reading__________________________________

void Mazesimulate::Config_Reading::Loading_config_table(){//加载配置表
    Add_config("Map_length_X", &mazesimulate->Map_length_X);
    Add_config("Map_length_Y", &mazesimulate->Map_length_Y);
    Add_config("Map_Max_area", &mazesimulate->Map_Max_area);
    Add_config("Map_mmin_x", &mazesimulate->Map_Min_X);
    Add_config("Map_mmin_y", &mazesimulate->Map_Min_Y);
    Add_config("Max_number_logs", &mazesimulate->Max_number_logs);
    Add_config("Render_Port", &mazesimulate->Render_Port);
    Add_config("Calculation_interval", &mazesimulate->Calculation_interval);
}

void Mazesimulate::Config_Reading::First_time_Loading(){
try{

    Load_Config();//加载配置表

}catch(const std::runtime_error& e){
    std::string errorStr = e.what();//捕获异常转化为字符串
    mazesimulate->Log_class.cout_Log_system(e.what(), Log_Exit::Log_type::WARNING_ENUM);//输出警告日志
}

}


bool Mazesimulate::Config_Reading::Check_file(std::string File_name){//检查配置文件是否存在
return std::filesystem::exists(File_name);//检查配置文件是否存在
}

void Mazesimulate::Config_Reading::Add_config(std::string config_name, int* Address){
config.push_back(config_Address{config_name, Address});//添加配置信息
}

void Mazesimulate::Config_Reading::Generate_port_file(){//生成渲染端口配置文件

bool Create_or_not = true;//创建或删除端口配置文件标志


if(Check_file(configPort)){//如果端口配置文件存在

if(std::remove(configPort.c_str())){//删除端口配置文件
    
        mazesimulate->Log_class.cout_Log_system("Failed to delete file -> Cancel creation.", Log_Exit::Log_type::WARNING_ENUM);//输出错误调用退出
        Create_or_not = false;

    }
}

if(Create_or_not){//创建端口配置文件

nlohmann::json config_Port;

config_Port["port"] = mazesimulate->Render_Port;//设置渲染端口
config_Port["Password"] = mazesimulate->Render_Password;//设置密码

 std::ofstream file(configPort);//打开json文件

 if(file.is_open()){

    file << config_Port.dump(4);  // 带缩进的格式化输出
    file.close();//关闭json文件

}

}


}


void Mazesimulate::Config_Reading::Load_Config(){

//throw std::runtime_error("Test");//抛出异常测试
if(Check_file(configPath)){//如果配置文件存在

    Read_config();//读取配置文件并解析配置信息

}else{//如果配置文件不存在

    Create_Config();//创建配置文件并写入默认配置

}

    Generate_port_file();//生成端口文件

}

void Mazesimulate::Config_Reading::Create_Config(){//创建json文件 并且写入默认配置
if(Check_file(configPath)){//如果配置文件存在
mazesimulate->Log_class.cout_Log_system("The config file already exists - > Trying to delete the file...", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志
    if(std::remove(configPath.c_str())){//删除失败
    
        throw std::runtime_error("Failed to delete file " + configPath + " files");

    }else{
    
        mazesimulate->Log_class.cout_Log_system("Deleted " + configPath + " files", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志
    
    }
}

//创建json文件

if(config.size()){//检查配置表是否为空

nlohmann::json configJson;

    for(const auto& item : config){//遍历配置表
        if(item.Address != nullptr) {   //检查指针是否为空
            configJson[item.config_name] = *(item.Address);
        }else{
            configJson[item.config_name] = 0;  //默认处理
        }
    }


    std::ofstream out(configPath);


    if (out.is_open()) {
        out << configJson.dump(4);  // 带缩进的格式化输出
        out.close();

        mazesimulate->Log_class.cout_Log_system("Created " + configPath + " files", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志


    } else {


        throw std::runtime_error("Failed to create the " + configPath + " files");
        
    }

}else{

mazesimulate->Log_class.cout_Log_system("No matching table found. - > Cancel creating JSON", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志

}

}

void Mazesimulate::Config_Reading::Read_config(){//读取json文件并解析配置信息

std::ifstream inFile(configPath);//打开json文件
if(inFile.is_open()){

    nlohmann::json configJson;
    inFile >> configJson;   // 解析 JSON
    inFile.close();


    for(const auto& item : config){
    if(configJson.contains(item.config_name)){
        int newValue = configJson[item.config_name].get<int>();
        *(item.Address) = newValue;   //更新原变量
        }
    }



}else{
    mazesimulate->Log_class.cout_Log_system("Unable to open file " + configPath + " - > Launch using default configuration" , Log_Exit::Log_type::WARNING_ENUM);
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
Parameter_table.push_back({"-resetjsonport","Refresh port configuration file", [this](){mazesimulate->Config_class.Generate_port_file();}});//添加默认重置json端口参数

Parameter_table.push_back({"-listmethods","List the algorithms", [this](){List_algorithms();}});
Parameter_table.push_back({"-make","Run the generation algorithm", [this](){make_algorithm_started();}});
Parameter_table.push_back({"-find","Run a search algorithm", [this](){find_Algorithm_started();}});

Parameter_table.push_back({"-methodlog","List the method log and clear the container", [this](){Output_Clear_Algorithm_Log();}});

config.push_back({"mapx", "Map X length", &mazesimulate->Map_length_X});
config.push_back({"mapy", "Map Y length", &mazesimulate->Map_length_Y});
config.push_back({"mapmax","Map Max 'area'", &mazesimulate->Map_Max_area});
config.push_back({"mapmminx", "Map Min X length", &mazesimulate->Map_Min_X});
config.push_back({"mapmminy", "Map Min Y length", &mazesimulate->Map_Min_Y});
config.push_back({"make","Choose the number of the generation method", &mazesimulate->Selected_generation_method});
config.push_back({"find","Choose the number of the search method", &mazesimulate->Selected_search_method});
config.push_back({"port", "Graphical rendering port", &mazesimulate->Render_Port});
config.push_back({"interval", "Calculation interval", &mazesimulate->Calculation_interval});
config.push_back({"render_Password", "rendering Port password(Stops working after modification)", &mazesimulate->Render_Password});
config.push_back({"Choose_make","Chosen generation algorithm", &mazesimulate->Selected_generation_method});
config.push_back({"Choose_find","Chosen search algorithm", &mazesimulate->Selected_search_method});

}




void Mazesimulate::Input_processing::help_operate(){//帮助操作
std::cout << "Available parameters(Default action function):" << std::endl;
if(config.empty()){

    std::cout << "No default action registered." << std::endl;
    
}else{

  for(const auto& Parameter : Parameter_table){
    std::cout << "[\033[90m" << Parameter.name << "\033[0m]" << Parameter.Guide << std::endl;
    }  
}
//配置列表
std::cout << "Config Parameter Table:" << std::endl;
if(config.empty()){

    std::cout << "No configuration parameters available." << std::endl;

}else{

    for(const auto& config : config){
    std::cout << "[\033[90m" << config.config_name << "\033[0m]" << config.config_description << " - > " << *config.Address <<std::endl;
    }

}



}


void Mazesimulate::Input_processing::Resetjson_operation(){//重置json文件

mazesimulate->Log_class.cout_Log_system("Reset Config File", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志
mazesimulate->Config_class.Create_Config();//创建json文件 并且写入默认配置
mazesimulate->Log_class.cout_Log_system("Operation completed", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志

}
//void find_Algorithm_started()
void Mazesimulate::Input_processing::find_Algorithm_started(){
    //调用启动搜索算法
    mazesimulate->Algorithm_class.Run_find_algorithm(mazesimulate->Selected_search_method);//调用启动搜索算法
}

void Mazesimulate::Input_processing::make_algorithm_started(){
    //调用启动生成算法
    mazesimulate->Algorithm_class.Run_make_algorithm(mazesimulate->Selected_generation_method);//调用启动生成算法
}

void Mazesimulate::Input_processing::Output_Clear_Algorithm_Log(){
//输出算法日志
mazesimulate->Log_class.Export_history_log();//输出算法日志

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

std::vector<std::string> Mazesimulate::Input_processing::Analyze_raw_inputdata(std::string Input){

std::vector<std::string> result;//存储解析后的输入数据
std::string token;//临时存储每个字符串
std::istringstream iss(Input);//创建输入流

while(iss >> token){//读取输入流
result.push_back(token);//将读取的字符串存入结果容器
}


return result;//返回解析后的输入数据
};




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



void Mazesimulate::Input_processing::Processing_input(std::vector<std::string> Input){

for(const auto& Currently : Input){

Go_back Back_data = Matching_Parameter(Currently);//匹配参数

    if(Back_data.Find_the_mood){//判断输入参数是否找到

        Parameter_table[Back_data.Location].Function();//调用配置表中的动作函数

    }else{//未找到

        Matching_config_table(Transformation_Config(Currently));//匹配配置表

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

            Processing_input(Analyze_raw_inputdata(token));//处理输入数据(入口)

        }catch(const std::runtime_error& e){
            std::string errorStr = e.what();
            mazesimulate->Log_class.cout_Log_system(e.what(), Log_Exit::Log_type::WARNING_ENUM);//输出警告日志
        
        }
            

       std::cout << "> " ;
    }

    std::cout << "'-EOF'" << std::endl;
    mazesimulate->Log_class.Exit("Input EOF to exit", Log_Exit::Exit_type::OPERATIONAL_ENUM);//跳出程序

}

Mazesimulate::Input_processing::User_Input Mazesimulate::Input_processing::Transformation_Config(std::string Input){

std::string strPart;//字符串部分
int numPart;//数字部分


size_t pos = Input.find_first_of("0123456789");//找到第一个数字的位置
if(pos != std::string::npos){
    strPart = Input.substr(0, pos);  // 数字前的字符串
    numPart = std::stoi(Input.substr(pos));  // 数字部分
}else{
    strPart = Input;//如果输入没有数字则直接赋值给strPart
    numPart = 0;//默认值为0
}

return User_Input{strPart, numPart};//返回用户输入内容

}


void Mazesimulate::Input_processing::Matching_config_table(User_Input Input){
bool Invalid = false;//无效输入

for(const auto& config : config){
    
    if(Input.name == config.config_name){
        *config.Address = Input.value;//匹配配置信息并赋值
        Invalid = true;//设置无效输入计数器为真
        break;
    }
}

if(!Invalid){
//输出日志
std::stringstream Temporary_log;
Temporary_log << "Invalid input: " << Input.name << " - > " << Input.value;//输出日志
mazesimulate->Log_class.cout_Log_system(Temporary_log.str(), Log_Exit::Log_type::WARNING_ENUM);//输出警告日志

}



}


//__________________________________Data_Processing__________________________________

void Mazesimulate::Data_Processing::Initialize_data(){//初始化数据

mazesimulate->Input_class.Get_Input();//调用用户输入

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

    Map_route = Stats_route_data(Map_data::Map_type::ROUTE_ENUM);//统计路线格数量

    Map_route_proportion = (double)Map_route / (mazesimulate->Map_length_X * mazesimulate->Map_length_Y) * 100;//计算路线格比例


}

void Mazesimulate::Data_Processing::Statistics_After(){//统计数据后




}






//__________________________________Mazesimulate__________________________________


void Mazesimulate::Framework_Entry(){//初始化


Data_class.Initialize_data();//初始化数据


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
    try{

    make_algorithm[index](*mazesimulate);//运行生成算法

    mazesimulate->Map_class.add_map_status(Map_data::Map_Status::GENERATED_ENUM);//设置地图状态已生成

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
    try{

    find_algorithm[index](*mazesimulate);//运行搜索算法


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