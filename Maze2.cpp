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
    
        mazesimulate->Log_class.cout_Log_system("Unable to create port configuration file", Log_Exit::Log_type::WARNING_ENUM);//输出错误调用退出
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

}else{
    
    mazesimulate->Log_class.cout_Log_system("Unable to create port configuration file", Log_Exit::Log_type::WARNING_ENUM);//输出错误调用退出

}


}


}


void Mazesimulate::Config_Reading::Load_Config(){


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
    
        mazesimulate->Log_class.Exit("Failed to try to delete the file", Log_Exit::Exit_type::SYSTEM_ENUM);//输出错误调用退出
    
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


        mazesimulate->Log_class.Exit("Failed to create the file", Log_Exit::Exit_type::SYSTEM_ENUM);//输出错误日志

        
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


    case Exit_type::ERROR_ENUM://错误退出
        Logout_Information << "[\033[31mEXIT_ERROR\033[0m]" << exit_log << std::endl;
        //输出退出前的错误信息
        
        if(historical_logs.empty()){break;}//如果日志容器为空跳过
        std::cout << "[\033[38;5;214mERROR_LOG\033[0m]Output the last'" << historical_logs.size() << "'logs before exit" << std::endl; //输出最后的错误日志
        
        while(!historical_logs.empty()){
            std::cout << historical_logs.front();
            historical_logs.pop();
            }
        
}




std::cout << Logout_Information.str();//输出错误信息

exit(0);//退出程序

}



//__________________________________Initialize__________________________________

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
        
        Processing_input(Analyze_raw_inputdata(token));//处理输入数据

       std::cout << "> " ;
    }
    std::cout << "-EOF" << std::endl;
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


//add_config_table
void Mazesimulate::Input_processing::add_config_table(std::string config_name, std::string config_description, int* Address){
    config.push_back(config_Address{config_name, config_description, Address});//添加配置信息
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
Temporary_log << "Invalid input: " << Input.name << " - > " << Input.value << std::endl;
mazesimulate->Log_class.cout_Log_system(Temporary_log.str(), Log_Exit::Log_type::WARNING_ENUM);//输出错误日志

}



}


//__________________________________Data_Processing__________________________________

void Mazesimulate::Data_Processing::Initialize_data(){//初始化数据
mazesimulate->Data_class.Add_config_table();//注册配置表信息
mazesimulate->Config_class.Load_Config();//加载配置文件

mazesimulate->Input_class.Get_Input();//调用用户输入

}


void Mazesimulate::Data_Processing::Add_config_table(){//注册配置表信息
mazesimulate->Input_class.add_config_table("mapx", "Map X length", &mazesimulate->Map_length_X);
mazesimulate->Input_class.add_config_table("mapy", "Map Y length", &mazesimulate->Map_length_Y);
mazesimulate->Input_class.add_config_table("Port", "Graphical rendering port", &mazesimulate->Render_Port);
mazesimulate->Input_class.add_config_table("interval", "Calculation interval", &mazesimulate->Calculation_interval);
mazesimulate->Input_class.add_config_table("Render_Password", "rendering Port password(Stops working after modification)", &mazesimulate->Render_Password);

mazesimulate->Config_class.Add_config("Map_length_X", &mazesimulate->Map_length_X);
mazesimulate->Config_class.Add_config("Map_length_Y", &mazesimulate->Map_length_Y);
mazesimulate->Config_class.Add_config("Max_number_logs", &mazesimulate->Max_number_logs);
mazesimulate->Config_class.Add_config("Render_Port", &mazesimulate->Render_Port);
mazesimulate->Config_class.Add_config("Calculation_interval", &mazesimulate->Calculation_interval);

}


//Stats_route_data


int Mazesimulate::Data_Processing::Stats_route_data(Mazesimulate::Map_data::Map_type map_type){//统计路线数据
int Temporary_statistics = 0;//临时统计路线数据
    for(int i = 0; i < mazesimulate->Map_length_X; i++){
        for(int j = 0; j < mazesimulate->Map_length_Y; j++){
            if(mazesimulate->Map_class.Check_a_tag(j, i, map_type)){
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


//__________________________________Map_data__________________________________
void Mazesimulate::Map_data::Initialize_map_data(Map_type Initialization_Type){//初始化地图数据

map_data.assign(mazesimulate->Map_length_X, std::vector<Map_type>(mazesimulate->Map_length_Y, Initialization_Type));//初始化地图数据


}


std::vector<std::vector<Mazesimulate::Map_data::Map_type>> Mazesimulate::Map_data::Get_map_data(){//获取地图数据

return map_data;//返回地图数据

}



bool Mazesimulate::Map_data::Check_location_range(int x, int y){//检查坐标是否在地图范围内

return (x >= 0 && x < mazesimulate->Map_length_X) && (y >= 0 && y < mazesimulate->Map_length_Y);

}

bool Mazesimulate::Map_data::Check_a_tag(int x, int y , Map_type a_tag){
if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
mazesimulate->Log_class.Exit("[Check_a_tag]Accessing the map beyond its size", Log_Exit::Exit_type::ERROR_ENUM);//输出错误日志
}
return map_data[x][y] == a_tag;//检查坐标是否包含指定的标记
}

bool Mazesimulate::Map_data::Check_the_finish(int x, int y){
if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
mazesimulate->Log_class.Exit("[Check_the_finish]Accessing the map beyond its size", Log_Exit::Exit_type::ERROR_ENUM);//输出错误日志
}

return x == finish_position.x && y == finish_position.y;//;//检查坐标是否包含指定的标记

}

bool Mazesimulate::Map_data::Check_the_start(int x, int y){
if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
mazesimulate->Log_class.Exit("[Check_the_start]Accessing the map beyond its size", Log_Exit::Exit_type::ERROR_ENUM);//输出错误日志
}

return x == start_position.x && y == start_position.y;//检查坐标是否包含指定的标记

}

Mazesimulate::Map_data::Map_type Mazesimulate::Map_data::Get_location_tag(int x, int y){

    if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
        mazesimulate->Log_class.Exit("[Get_location_tag]Accessing the map beyond its size", Log_Exit::Exit_type::ERROR_ENUM);//输出错误日志
    }

    return map_data[x][y];//返回地图位置标签数据

}

void Mazesimulate::Map_data::Set_location_tag(int x, int y, Map_type a_tag){//设置地图位置标签数据
    if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
        mazesimulate->Log_class.Exit("[Set_location_tag]Accessing the map beyond its size", Log_Exit::Exit_type::ERROR_ENUM);//输出错误日志
    }

    map_data[x][y] = a_tag;//设置地图位置标签数据

}

void Mazesimulate::Map_data::Set_finish_position(int x, int y){//设置终点位置

    if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
        mazesimulate->Log_class.Exit("[Set_finish_position]Accessing the map beyond its size", Log_Exit::Exit_type::ERROR_ENUM);//输出错误日志
    }

finish_position.x = x;
finish_position.y = y;
}

void Mazesimulate::Map_data::Set_start_position(int x, int y){//设置起点位置

    if(!Check_location_range(x, y)){//检查坐标是否在地图范围内
        mazesimulate->Log_class.Exit("[Set_start_position]Accessing the map beyond its size", Log_Exit::Exit_type::ERROR_ENUM);//输出错误日志
    }

start_position.x = x;
start_position.y = y;
}

//___________________________________Render_Send___________________________________


void Mazesimulate::Render_Send::Settings_Render_Password(){
//生成1000-9999的随机数作为密码
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(1000, 9999);
mazesimulate->Render_Password = dis(gen);

}