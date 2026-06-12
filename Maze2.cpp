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


bool Mazesimulate::Config_Reading::Check_config_file(){//检查配置文件是否存在
return std::filesystem::exists(configPath);//检查配置文件是否存在
}

void Mazesimulate::Config_Reading::Add_config(std::string config_name, int* Address){
config.push_back(config_Address{config_name, Address});//添加配置信息
}




void Mazesimulate::Config_Reading::Load_Config(){


if(Check_config_file()){//如果配置文件存在

Read_config();//读取配置文件并解析配置信息

}else{//如果配置文件不存在

Create_Config();//创建配置文件并写入默认配置

}




}

void Mazesimulate::Config_Reading::Create_Config(){//创建json文件 并且写入默认配置

if(Check_config_file()){//如果配置文件存在
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

/*

NORMAL_ENUM,//通知日志
ERROR_ENUM,//错误日志
PROCESS_ENUM,//处理日志(算法处理流程)
INITIAL_ENUM,//初始化日志(数据初始化)

*/

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

std::cout << "> " ;

}


void Mazesimulate::Input_processing::Resetjson_operation(){//重置json文件

mazesimulate->Log_class.cout_Log_system("Reset Config File", Log_Exit::Log_type::NOTICE_ENUM);//输出通知日志
mazesimulate->Config_class.Create_Config();//创建json文件 并且写入默认配置
mazesimulate->Log_class.Exit("Config file reset - > Restart to take effect", Log_Exit::Exit_type::OPERATIONAL_ENUM);


}









Mazesimulate::Input_processing::Parameter Mazesimulate::Input_processing::Matching_Parameter(std::string Input){//匹配参数

Parameter Go_back = Parameter::NOT_FOUND_ENUM;//设置默认参数为未找到

for(const auto& Parameter : Parameter_table){

if(Parameter.name == Input){//如果输入参数与配置表中的参数匹配
Parameter.Function();//调用参数函数
Go_back = (Parameter.is_end_input) ? Parameter::END_INPUT_ENUM : Parameter::MATCHING_ENUM;//设置匹配参数为匹配或结束输入
break;//跳出循环
}

}

return Go_back;//返回匹配的参数
}



void Mazesimulate::Input_processing::Get_Input(){

    std::vector<std::string> inputs; //临时存储输入的容器
    std::string token; //临时存储每个字符串

            std::cout << "Enter parameters to start the simulation" << std::endl;
            std::cout << "[type '-help' to see available parameters]" << std::endl;
            std::cout << "> ";

/*
                    NOT_FOUND_ENUM,//未找到(给配置函数传入)
                    MATCHING_ENUM,//匹配(操作)
                    END_INPUT_ENUM,//结束输入
*/

        bool End_input = false;//设置默认结束输入为false
        while(std::cin >> token){

            switch(Matching_Parameter(token)){

                case Parameter::NOT_FOUND_ENUM://未找到(给配置函数传入)
                inputs.push_back(token);//将输入内容存入inputs容器
                break;

                case Parameter::MATCHING_ENUM://匹配(操作)
                break;

                case Parameter::END_INPUT_ENUM://结束输入
                End_input = true;
                break;

            }

            if(End_input){break;}//如果结束输入则跳出循环

        }


    for(const auto& input : inputs){//处理输入内容
        std::string strPart;//字符串部分
        int numPart;//数字部分


        size_t pos = input.find_first_of("0123456789");//找到第一个数字的位置
        if(pos != std::string::npos){
            strPart = input.substr(0, pos);  // 数字前的字符串
            numPart = std::stoi(input.substr(pos));  // 数字部分
        }else{
            strPart = input;//如果输入没有数字则直接赋值给strPart
            numPart = 0;//默认值为0
        }

        UserInput.push_back(User_Input{strPart, numPart});//将输入内容存入UserInput容器
        //调用日志
        std::stringstream Temporary_log;
        Temporary_log << "User input parameters: " << strPart << "," << numPart;
    
        mazesimulate->Log_class.cout_Log_system(Temporary_log.str(), Log_Exit::Log_type::PROMPT_ENUM);//输出提示日志
    }

    Matching_config_table();//匹配配置表

}

//add_config_table
void Mazesimulate::Input_processing::add_config_table(std::string config_name, std::string config_description, int* Address){
    config.push_back(config_Address{config_name, config_description, Address});//添加配置信息
}


//Matching_config_table
void Mazesimulate::Input_processing::Matching_config_table(){
int Invalid_input = 0;
for(const auto& UserInput : UserInput){
    bool match = false;//设置默认匹配为false
        for(const auto& config : config){
            
            if(UserInput.name == config.config_name){
                *config.Address = UserInput.value;//匹配配置信息并赋值
                match = true;
                break;
            }
        }

    if(!match){//统计无效输入
        Invalid_input++;
    }    

}

if(Invalid_input > 0){
//输出日志
std::stringstream Temporary_log;
Temporary_log << "Invalid input parameters: " << Invalid_input << std::endl;
mazesimulate->Log_class.cout_Log_system(Temporary_log.str(), Log_Exit::Log_type::WARNING_ENUM);//输出错误日志

}



}


//__________________________________Data_Processing__________________________________

void Mazesimulate::Data_Processing::Initialize_data(){//初始化数据
mazesimulate->Data_class.Add_config_table();//注册配置表信息
mazesimulate->Config_class.Load_Config();//加载配置文件

mazesimulate->Input_class.Get_Input();//调用用户输入

std::cout << mazesimulate->Map_length_X << " " << mazesimulate->Map_length_Y << std::endl; //尝试输出地图长度




//mazesimulate->Log_class.Exit("Initialization completed", Log_Exit::Exit_type::ERROR_ENUM);
}


void Mazesimulate::Data_Processing::Add_config_table(){//注册配置表信息
mazesimulate->Input_class.add_config_table("mapx", "Map X length", &mazesimulate->Map_length_X);//添加配置信息
mazesimulate->Input_class.add_config_table("mapy", "Map Y length", &mazesimulate->Map_length_Y);//添加配置信息
mazesimulate->Input_class.add_config_table("Port", "Graphical rendering port", &mazesimulate->Render_Port);
mazesimulate->Input_class.add_config_table("interval", "Calculation interval", &mazesimulate->Calculation_interval);//添加配置信息

mazesimulate->Config_class.Add_config("Map_length_X", &mazesimulate->Map_length_X);//添加配置信息
mazesimulate->Config_class.Add_config("Map_length_Y", &mazesimulate->Map_length_Y);//添加配置信息
mazesimulate->Config_class.Add_config("Max_number_logs", &mazesimulate->Max_number_logs);//添加配置信息
mazesimulate->Config_class.Add_config("Render_Port", &mazesimulate->Render_Port);
mazesimulate->Config_class.Add_config("Calculation_interval", &mazesimulate->Calculation_interval);

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