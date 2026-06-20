#ifndef MAZEINFRA
#define MAZEINFRA

#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <unordered_map>
#include <queue>
#include <variant>

#include <iostream>
#include <limits>  // numeric_limits
#include <sstream>
#include <memory>
#include <algorithm>  // shuffle
#include <random>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "typeenum.hpp"//包含类型枚举

#include "third_party\kissnet.hpp"
#include "third_party\json.hpp"
#include "third_party\process\process.hpp"

namespace kn = kissnet;//使用kissnet命名空间




class String_manipulation : public datatype{

    private:
    /*data*/

        bool isFloat(const std::string& s){
            if(s.empty()){return false;}//如果字符串为空，返回false
            std::istringstream iss(s);
            double d;
            iss >> std::noskipws >> d;   // 不跳过空白，严格匹配
            // 检查是否读取成功且所有字符都被消耗
            return (iss.eof() && !iss.fail());/* && (s[0] != '.' && s[s.size()-1] != '.')*/
        }


    protected://保护类

std::string variantToString(const std::variant<int, float, std::string, bool>& v){//将variant类型转换为string类型
    switch(v.index()){
        case 0: { // int
            return std::to_string(std::get<int>(v));
        }
        case 1: { // float
            std::ostringstream oss;
            oss << std::get<float>(v); // 默认流格式，去掉尾随零
            return oss.str();
        }
        case 2: { // string
            return std::get<std::string>(v);
        }
        case 3: { // bool
            return std::get<bool>(v) ? "true" : "false";
        }
        default:
            return ""; // 理论上不会发生
    }
}


    std::variant<int, float, std::string, bool> ToValueVariant(const std::variant<int*, float*, std::string*, bool*>& ptrVar){//将指针转换为variant类型
        if (auto* p = std::get_if<int*>(&ptrVar)) {
            return **p;   // 注意：std::get_if 返回的是指向指针的指针，所以要两次解引用
        } else if (auto* p = std::get_if<float*>(&ptrVar)) {
            return **p;
        } else if (auto* p = std::get_if<std::string*>(&ptrVar)) {
            return **p;
        } else if (auto* p = std::get_if<bool*>(&ptrVar)) {
            return **p;
        }
        throw std::bad_variant_access();
    }

    std::vector<std::string> Transform_str_vec(std::string Input){//将字符串转换为容器
        if(Input.empty()){return {};}//如果输入为空，返回空容器
        std::vector<std::string> result;//存储解析后的输入数据
        std::string token;//临时存储每个字符串
        std::istringstream iss(Input);//创建输入流
            
        while(iss >> token){//读取输入流
        result.push_back(token);//将读取的字符串存入结果容器
        }
        return result;//返回解析后的输入数据
    };

    
    std::string vec_to_str(std::vector<std::string> Input){//将容器转换为字符串
        std::stringstream iss;//创建输入流1
        for(auto& str : Input){//遍历输入容器
            iss << str << " ";//将每个字符串写入输入流1
        }
        return iss.str();
    }

    std::pair<std::string, std::string> splittwo(std::string str, char delimiter){//根据分隔符将字符串分割成两部分
        std::string strPart, numPart;
    
        size_t pos = str.find_first_of(delimiter);//找到第一个分隔符的位置
        if(pos == 0){//分割符是第一个字符，则标识部分为空 
            return std::make_pair("", "");
        }
        if(pos != std::string::npos){
            strPart = str.substr(0, pos);  //标识部分
            numPart = str.substr(pos+1);  //参数部分
        }else{
            strPart = str;//如果输入没有数字则直接赋值给strPart
            numPart = "";
        }
        
        return std::make_pair(strPart, numPart);
    }


    int Count_str_characters(std::string Input, std::string charset){//计算字符串中字符的个数
        int count = 0;
        for(char ch : Input){
            if(charset.find(ch) != std::string::npos){//如果字符是目标字符
                count++; //计数器加1
            }
        }
        return count;
    }


    Data_type analyzing_type(std::string Input){//分析字符串类型

        if(Input.empty()){return Data_type::NULL_ENUM;}//如果输入为空，返回NULL_ENUM

        if(Input.find_first_not_of("0123456789") == std::string::npos){//如果输入全是数字
            return Data_type::INT_ENUM;//返回整数枚举
        }else if(isFloat(Input)){
            return Data_type::FLOAT_ENUM;//返回浮点数枚举
        }else if(Input == "true" || Input == "false"){
            return Data_type::BOOL_ENUM;//返回布尔枚举
        }else{
            return Data_type::STRING_ENUM;//返回字符串枚举
        }
        return Data_type::NULL_ENUM;//未知数据类型
    }

    std::string Type_Name(Data_type Input){//获取数据类型名称
        switch(Input){

            case Data_type::INT_ENUM:
                return "\033[2mINT_TYPE\033[0m";
            break;

            case Data_type::FLOAT_ENUM:
                return "\033[2mFLOAT_TYPE\033[0m";
            break;

            case Data_type::BOOL_ENUM:
                return "\033[2mtrue/false\033[0m";
            break;

            case Data_type::STRING_ENUM:
                return "\033[2mSTRING_TYPE\033[0m";
            break;

            default:
                return "\033[2mUNKNOWN\033[0m";

        }
    }
    //Convert_to_string variantToString

    void InputType_Conversion(std::string Input, std::variant<int*, float*, std::string*, bool*> ptrVar , Data_type Input_type){

        switch(Input_type){
            case Data_type::INT_ENUM://整数
                *std::get<int*>(ptrVar) = std::stoi(Input);
            break;

            case Data_type::FLOAT_ENUM://浮点数
                *std::get<float*>(ptrVar) = std::stof(Input);
            break;

            case Data_type::STRING_ENUM://字符串
                *std::get<std::string*>(ptrVar) = Input;
            break;

            case Data_type::BOOL_ENUM://布尔
                *std::get<bool*>(ptrVar) = (Input == "true" || Input == "TRUE" || Input == "1");
            break;

            default:
                throw std::runtime_error("Unknown data type in input");
        }

    }



};


class json_ConfigRelated :public String_manipulation{
    protected://保护类
        struct config_Address{//配置表
            std::string config_name;//配置名
            std::variant<int*, float*, std::string*, bool*> Address;//配置地址
            Data_type data_type;//数据类型
        };



    private:
    /*data*/
    protected://保护类
    
    bool Check_file(std::string File_name){//检查配置特定的文件是否存在
        return std::filesystem::exists(File_name);
    }

    nlohmann::json Open_json_file(std::string File_name){//打开配置文件并返回读取数据
        std::ifstream inFile(File_name);
        if(!inFile.is_open()){
            throw std::runtime_error("Failed to open file: " + File_name);
        }
        nlohmann::json config;
        inFile >> config;
        inFile.close();
        return config;
    }

    nlohmann::json Create_json_through(std::vector<config_Address> Config_table){//通过配置表创建 nlohmann::json对象
        nlohmann::json config;
        for(auto& Variable : Config_table){
            switch(Variable.data_type){
                case Data_type::INT_ENUM://整数
                    config[Variable.config_name] = *std::get<int*>(Variable.Address);
                break;

                case Data_type::FLOAT_ENUM://浮点数
                    config[Variable.config_name] = *std::get<float*>(Variable.Address);
                break;

                case Data_type::STRING_ENUM://字符串
                    config[Variable.config_name] = *std::get<std::string*>(Variable.Address);
                break;

                case Data_type::BOOL_ENUM://布尔
                    config[Variable.config_name] = *std::get<bool*>(Variable.Address);
                break;

                case Data_type::NULL_ENUM://未知数据类型
                    throw std::runtime_error("Unknown data type in config table");
                break;

            }
        }
        return config;
    }

    void Update_config(std::vector<config_Address> Config_table, nlohmann::json config){//通过 nlohmann::json对象更新配置表中的数据(指针)

        for(auto& Variable : Config_table){
            if(config.contains(Variable.config_name)){
                switch(Variable.data_type){
                    case Data_type::INT_ENUM://整数
                        *std::get<int*>(Variable.Address) = config[Variable.config_name].get<int>();
                    break;

                    case Data_type::FLOAT_ENUM://浮点数
                        *std::get<float*>(Variable.Address) = config[Variable.config_name].get<float>();
                    break;

                    case Data_type::STRING_ENUM://字符串
                        *std::get<std::string*>(Variable.Address) = config[Variable.config_name].get<std::string>();
                    break;

                    case Data_type::BOOL_ENUM://布尔
                        *std::get<bool*>(Variable.Address) = config[Variable.config_name].get<bool>();
                    break;

                    case Data_type::NULL_ENUM://未知数据类型
                        throw std::runtime_error("Unknown data type in config table");

                }
            }
        }
    }


    void Clean_files(std::string File_name){//清理文件
    if(Check_file(File_name)){//如果文件存在
    if(std::remove(File_name.c_str())){//删除失败
        throw std::runtime_error("Failed to delete file " + File_name + " files");
            }
        }        
    }


    void Write_file(std::string File_name, nlohmann::json config){//写入配置文件
        Clean_files(File_name);//清理文件

        std::ofstream outFile(File_name);//创建文件    
        if(outFile.is_open()){
            outFile << config.dump(4);  // 带缩进的格式化输出
            outFile.close();
        }else{
            throw std::runtime_error("Failed to create the " + File_name + " files");
        }

    }

};












class Communication : public String_manipulation{

    protected://保护类

    struct network_object{
        std::string ip_address = "127.0.0.1";//IP地址
        kn::port_t Send_port = 0000;//发送端口
        kn::port_t Receive_port = 0001;//接收端口
    };

    struct Transferdata{
        std::string Sign;//标识符
        std::vector<std::string> data;//数据
    };


    private:
    /*data*/


        kn::udp_socket Receive;//创建接收对象
        kn::udp_socket Send;//创建发送对象
        network_object Network_settings;//网络设置


    protected://保护类


    void Create_network_object(network_object Network_settings){//创建网络对象
        Receive = kn::udp_socket(kn::endpoint{Network_settings.ip_address, Network_settings.Receive_port});//创建接收对象
        Send = kn::udp_socket(kn::endpoint{Network_settings.ip_address, Network_settings.Send_port});//创建发送对象
    }


        void Send_Data(std::string data){//发送数据
            Send.send(
                reinterpret_cast<const std::byte*>(data.data()),
                data.size()
            );
        }



        std::string Receive_Data(){
            kn::buffer<1024> buf;
            auto [len, status] = Receive.recv(buf);
            if(status && len > 0){
                // 将收到的字节还原成 std::string
                std::string received(
                    reinterpret_cast<const char*>(buf.data()),
                    len
                );
                return received;
        }
        return "";
        }


        void Send_data_string(std::string Sign, std::vector<std::string> data){//发送数据
            Send_Data(Sign + vec_to_str(data));//发送数据
        }


        Transferdata Receive_data_string(){//接收数据
            std::vector<std::string> data = Transform_str_vec(Receive_Data());//接收数据

            std::string sign = data[0];
            data.erase(data.begin());//删除标识符

            return Transferdata{sign, data};
        }

        void Send_Status(std::string Sign ,bool status){//发送状态
            std::string status_str = status ? "true" : "false";
            Send_Data(Sign + "." + status_str);//发送状态
        }

        int Receive_Status(std::string Sign){//接收状态
            std::pair<std::string, std::string> Parse_receive =  splittwo(Receive_Data(),'.');//根据分隔符将字符串分割成两部分
            if(Parse_receive.first == Sign){

                if(Parse_receive.second == "true"){
                    return 1;
                }else{
                    return 0;
                }
            }else{
                return -1;
            }
        }



};




#endif// MAZEINFRA