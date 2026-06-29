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
#include "MazePluginAPI.h"//包含MazePluginAPI头文件

#include "third_party\kissnet.hpp"
#include "third_party\json.hpp"
#include "third_party\proc_spawn.hpp"
#include "third_party\tc_wait.hpp"
#include "third_party\dylib.hpp"

#if _WIN32
    std::string ext = ".dll";
#else
    std::string ext = ".so";
    #include <termios.h>
    #include <unistd.h>
#endif



namespace kn = kissnet;//使用kissnet命名空间
//继承logtype
class Lowlevel_logs{//低级日志类

    private:
    bool Get_number_row = false;
    int row_cout = 0;
    int col_cout = 0;
    public:

    void Show_log(std::string log_message,std::string Belonging){//日志输出函数
        std::cout << "[" << Belonging << "] " << log_message << std::endl;
        if(Get_number_row){
            if(!get_cursor_position(row_cout,col_cout)){
                std::cout << "[log_layer]" << "Unable to get terminal row; column" << std::endl;
            }
            Get_number_row = true;
        }
    }

    void Undo_log_cout(){
        int row = 0;
        if(!get_cursor_position(row,col_cout)){
            std::cout << "[log_layer]" << "Unable to get terminal row; column" << std::endl;
            return;
        }
        int Clear_row = row_cout - row;

        for(int i = 0; i < Clear_row; i++){
            std::cout << "\033[2K";//清除行
            std::cout << "\033[A" ; // 向上移动一行
        }

    }

    


};









class String_manipulation {

    private:
    /*data*/
    //datatype datatype;//字符串处理类
        bool isFloat(const std::string& s){
            if(s.empty()){return false;}//如果字符串为空，返回false
            std::istringstream iss(s);
            double d;
            iss >> std::noskipws >> d;   // 不跳过空白，严格匹配
            // 检查是否读取成功且所有字符都被消耗
            return (iss.eof() && !iss.fail());/* && (s[0] != '.' && s[s.size()-1] != '.')*/
        }


    //protected://保护类
    public:

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


    datatype::Data_type analyzing_type(std::string Input){//分析字符串类型

        if(Input.empty()){return datatype::Data_type::NULL_ENUM;}//如果输入为空，返回NULL_ENUM

        if(Input.find_first_not_of("0123456789") == std::string::npos){//如果输入全是数字
            return datatype::Data_type::INT_ENUM;//返回整数枚举
        }else if(isFloat(Input)){
            return datatype::Data_type::FLOAT_ENUM;//返回浮点数枚举
        }else if(Input == "true" || Input == "false"){
            return datatype::Data_type::BOOL_ENUM;//返回布尔枚举
        }else{
            return datatype::Data_type::STRING_ENUM;//返回字符串枚举
        }
        return datatype::Data_type::NULL_ENUM;//未知数据类型
    }

    std::string Type_Name(datatype::Data_type Input){//获取数据类型名称
        switch(Input){

            case datatype::Data_type::INT_ENUM:
                return "\033[2mINT_TYPE\033[0m";
            break;

            case datatype::Data_type::FLOAT_ENUM:
                return "\033[2mFLOAT_TYPE\033[0m";
            break;

            case datatype::Data_type::BOOL_ENUM:
                return "\033[2mtrue/false\033[0m";
            break;

            case datatype::Data_type::STRING_ENUM:
                return "\033[2mSTRING_TYPE\033[0m";
            break;

            default:
                return "\033[2mUNKNOWN\033[0m";

        }
    }
    //Convert_to_string variantToString

    void InputType_Conversion(std::string Input, std::variant<int*, float*, std::string*, bool*> ptrVar , datatype::Data_type Input_type){//将字符串转换为指针并转换为variant类型

        switch(Input_type){
            case datatype::Data_type::INT_ENUM://整数
                *std::get<int*>(ptrVar) = std::stoi(Input);
            break;

            case datatype::Data_type::FLOAT_ENUM://浮点数
                *std::get<float*>(ptrVar) = std::stof(Input);
            break;

            case datatype::Data_type::STRING_ENUM://字符串
                *std::get<std::string*>(ptrVar) = Input;
            break;

            case datatype::Data_type::BOOL_ENUM://布尔
                *std::get<bool*>(ptrVar) = (Input == "true" || Input == "TRUE" || Input == "1");
            break;

            default:
                throw std::runtime_error("Unknown data type in input");
        }

    }

    std::vector<int> Convert_string_array_integer(std::vector<std::string> Input){//将字符串数组转换为整数数组
        std::vector<int> result;
        for(auto& str : Input){
            int num = std::stoi(str);
            result.push_back(num);
        }
        return result;
    }

    std::vector<std::string> Convert_integer_array_string(std::vector<int> Input){
        std::vector<std::string> result;
        for(auto& str : Input){
            std::string num = std::to_string(str);
            result.push_back(num);
        }

        return result;
    }




};


class json_ConfigRelated {
    private:
    //String_manipulation string_Analysis;//字符串处理类   
    //protected://保护类
    Lowlevel_logs log;//日志类
    public://配置表

        struct config_Address{//配置表
            std::string config_name;//配置名
            std::variant<int*, float*, std::string*, bool*> Address;//配置地址
            datatype::Data_type data_type;//数据类型
        };

    
    bool Check_file(std::string File_name){//检查配置特定的文件是否存在
        return std::filesystem::exists(File_name);
    }

    nlohmann::json Open_json_file(std::string File_name){//打开配置文件并返回nlohmann::json对象数据
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
                case datatype::Data_type::INT_ENUM://整数
                    config[Variable.config_name] = *std::get<int*>(Variable.Address);
                break;

                case datatype::Data_type::FLOAT_ENUM://浮点数
                    config[Variable.config_name] = *std::get<float*>(Variable.Address);
                break;

                case datatype::Data_type::STRING_ENUM://字符串
                    config[Variable.config_name] = *std::get<std::string*>(Variable.Address);
                break;

                case datatype::Data_type::BOOL_ENUM://布尔
                    config[Variable.config_name] = *std::get<bool*>(Variable.Address);
                break;

                case datatype::Data_type::NULL_ENUM://未知数据类型
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
                    case datatype::Data_type::INT_ENUM://整数
                        *std::get<int*>(Variable.Address) = config[Variable.config_name].get<int>();
                    break;

                    case datatype::Data_type::FLOAT_ENUM://浮点数
                        *std::get<float*>(Variable.Address) = config[Variable.config_name].get<float>();
                    break;

                    case datatype::Data_type::STRING_ENUM://字符串
                        *std::get<std::string*>(Variable.Address) = config[Variable.config_name].get<std::string>();
                    break;

                    case datatype::Data_type::BOOL_ENUM://布尔
                        *std::get<bool*>(Variable.Address) = config[Variable.config_name].get<bool>();
                    break;

                    case datatype::Data_type::NULL_ENUM://未知数据类型
                        throw std::runtime_error("Unknown data type in config table");

                }
            }
        }
    }


    void Clean_files(std::string File_name){//清理文件
    if(Check_file(File_name)){//如果文件存在
    log.Show_log("Cleaning file " + File_name + " files", "json_ConfigRelated"); // 输出日志信息
    if(std::remove(File_name.c_str())){//删除失败
        throw std::runtime_error("Failed to delete file " + File_name + " files");
            }
        log.Undo_log_cout();//清除日志
        }        
    }


    void Write_file(std::string File_name, nlohmann::json config){//写入配置文件
        log.Show_log("Check if the file already exists...", "json_ConfigRelated");
        Clean_files(File_name);//清理文件

        std::ofstream outFile(File_name);//创建文件
        log.Show_log("File created " + File_name + " files", "json_ConfigRelated");
        if(outFile.is_open()){
            outFile << config.dump(4);  // 带缩进的格式化输出
            outFile.close();
        }else{
            throw std::runtime_error("Failed to create the " + File_name + " files");
        }

    }

};












class Communication{//通信类



    private://私有成员
    /*data*/
        kn::udp_socket Receive;//创建接收对象
        kn::udp_socket Send;//创建发送对象
        String_manipulation string_Analysis;//字符串处理类
        int Timeout_waiting_period = 100;//超时时间


        enum class Connection_Status{
            GOT_IT_TRUE,//收到 真
            GOT_IT_NULL,//收到 假
            NO_MATCH,//不匹配
            NO_RECEIVE,//未收到数据
        };


        void Send_Data(std::string data){//发送数据
            Send.send(
                reinterpret_cast<const std::byte*>(data.data()),
                data.size()
            );
        }

        std::string Timeout_reading_data(){//超时时间接收数据
        
            auto status = Receive.select(kn::fds_read, Timeout_waiting_period);

            if(status == kn::socket_status::valid){
                kn::buffer<5024> buf;
                auto [len, recv_status] = Receive.recv(buf);
                    if(recv_status && len > 0){
                        // 将收到的字节还原成 std::string
                        std::string received(
                            reinterpret_cast<const char*>(buf.data()),
                            len
                        );
                        return received;
                    }

            }

            return "";
        }




    //protected://保护类
    public:

    using port = kn::port_t;//使用kissnet命名空间
    using Network_Status = Connection_Status;//状态枚举
    struct Transferdata{
        std::string Sign;//标识符
        std::vector<std::string> data;//数据
    };



    void Create_network_object(std::string ip_address,int timeout_waiting_period, port Send_port, port Receive_port){//创建网络对象
        Receive = kn::udp_socket(kn::endpoint{ip_address, Receive_port});//创建接收对象
        Send = kn::udp_socket(kn::endpoint{ip_address, Send_port});//创建发送对象
        Timeout_waiting_period = timeout_waiting_period;//设置超时时间
        try{//绑定接收对象
        Receive.bind();//绑定接收对象
        }catch(std::runtime_error& e){
            std::cerr << "Failed to bind the udp socket: " << e.what() << std::endl;
            exit(0);
        }
    }


    void Send_Single_state(std::string Sign){//发送标识符
        Send_Data(Sign);//发送数据
    }

    bool Receive_Single_state(std::string Sign){//接收标识符
        std::string received = Timeout_reading_data();//接收数据
        return (received == Sign);//判断是否收到数据
    }

    void Send_data(std::string Sign, std::string data){
        Send_Data(Sign + " " + data);//发送数据
    }

    Transferdata Receive_data(){//接收数据
        std::vector<std::string> data = string_Analysis.Transform_str_vec(Timeout_reading_data());//接收数据
        if(data.empty()){return Transferdata{};}
        std::string sign = data[0];//标识符
        data.erase(data.begin());//删除标识符
        return Transferdata{sign, data};
    }

    void Send_status(std::string Sign ,bool status){//发送状态
        std::string status_str = status ? "true" : "false";
        Send_Data(Sign + "." + status_str);//发送状态
    }

    Network_Status Receive_status(std::string Sign){//接收状态
        std::pair<std::string, std::string> Parse_receive =  string_Analysis.splittwo(Timeout_reading_data(),'.');//根据分隔符将字符串分割成两部分
        if(Parse_receive.second.empty()){return Network_Status::NO_RECEIVE;}//未收到数据
        if(Parse_receive.first == Sign){
            if(Parse_receive.second == "true"){
                return Network_Status::GOT_IT_TRUE;
            }else if(Parse_receive.second == "false"){
                return Network_Status::GOT_IT_NULL;
            }
        }
        return Network_Status::NO_MATCH;
    }

    void Send_individual_data(std::string Sign, std::string data){//发送单个数据
        Send_Data(Sign + "." + data);//发送数据
    }

    std::pair<std::string, std::string> Receive_individual_data(){//接收单个数据
        return string_Analysis.splittwo(Timeout_reading_data(),'.');
    }



};

class algorithm_DLL{//算法DLL类

    public://算法信息结构体

    enum class algorithmtype{
        UNKNOWN_ENUM = 0, //未知
        CREATE_ENUM = 1,
        SEARCH_ENUM = 2
    };



    struct Algorithm_Info{//算法信息结构体
        std::string name = "null";//算法名
        std::string Author = "null";//作者
        std::string description = "null";//算法介绍
        algorithmtype type = algorithmtype::UNKNOWN_ENUM;//算法类型
    };

    struct Algorithm_Function{
        Algorithm_Info info;//算法信息
        std::unique_ptr<dylib::library> lib;//加载DLL
        std::function<int(const MazePluginAPI* api)> Initialize;//算法初始化函数
        std::function<void()> function;//算法函数
        std::function<void()> Clean_up;//算法清理函数
        bool Function_Effect = false;//算法dll函数是否有效
    };

    private://私有成员
    /*data*/


    MazePluginAPI interface = {nullptr};//算法接口
    
    //std::vector<Algorithm_Function> Algorithm_Table;//算法函数表

    public:

        std::vector<std::string> Scan_Document(std::string Document_path){
            namespace fs = std::filesystem;
            std::vector<std::string> dllFiles; // 存储所有DLL文件的路径

            std::string targetDir = Document_path;//目标目录
            std::string extension = ext;//文件扩展名

            if((!fs::exists(Document_path))){
                 fs::create_directories(Document_path);
                 /*日志*/
            }

            for (const auto& entry : fs::recursive_directory_iterator(targetDir)) {
                if (entry.is_regular_file() && entry.path().extension() == extension) {
                    std::cout << fs::absolute(entry.path()) << '\n';
                    dllFiles.push_back(fs::absolute(entry.path()).string()); // 将文件路径添加到vector中
                }
            }
            return dllFiles; // 返回所有DLL文件的路径
        }

        void InitializeAPI(MazePluginAPI& Function){//初始化API
            interface = Function;
        }

        std::vector<std::unique_ptr<dylib::library>> Initialize_DLL_object(std::vector<std::string> Document_path){//加载DLLs
        std::vector<std::unique_ptr<dylib::library>> lib_list;//加载DLLs
            for(auto& Document : Document_path){
                try{
                    auto lib = std::make_unique<dylib::library>(Document);//加载DLL
                    lib_list.push_back(std::move(lib));//将库对象添加到列表中
                }catch(std::runtime_error& e){
                    std::cerr << "[\033[31mFailed to load DLL\033[30m] " << Document << "," << e.what() << std::endl;
                }
            }
            return lib_list;//返回加载的DLLs
        }
        
        std::vector<Algorithm_Function> Analyze_DLL(std::vector<std::unique_ptr<dylib::library>>& lib_list){

            std::vector<Algorithm_Function> Algorithm_Table;//算法函数表

            for(auto& lib : lib_list){
                try{
                    Algorithm_Function algorithm;//算法信息

                    auto Plugin_Info = lib->get_function<Information()>("GetInformation");//获取dll算法信息

                    auto init_func = lib->get_function<int(const MazePluginAPI* api)>("plugin_init");//获取dll算法初始化函数
                    auto Plugin_Run = lib->get_function<void()>("Run_plugin");//获取dll算法运行函数
                    auto Final_cleanup = lib->get_function<void()>("Final_cleanup");//获取dll算法清理函数

                    bool Core_Function_Status = !(init_func == nullptr || Plugin_Run == nullptr || Final_cleanup == nullptr);
                    bool Information_Function_Status = !(Plugin_Info == nullptr);//信息函数是否有效
                    algorithm.Function_Effect = Core_Function_Status;//判断算法dll函数是否有效

                    if(Core_Function_Status){//如果算法dll函数有效
                        algorithm.lib = std::move(lib);//将库对象移动到算法信息表
                        algorithm.Initialize = init_func;//设置算法初始化函数
                        algorithm.function = Plugin_Run;//设置算法函数
                        algorithm.Clean_up = Final_cleanup;//设置算法清理函数
                    }


                    if(Information_Function_Status){//如果信息函数有效
                        Information info = Plugin_Info();//获取dll算法信息

                        algorithmtype type = //检查算法类型是否合法
                        (info.Type == CREATE_ENUM) ? algorithmtype::CREATE_ENUM : 
                        (info.Type == SEARCH_ENUM) ? algorithmtype::SEARCH_ENUM : 
                        algorithmtype::UNKNOWN_ENUM;

                        algorithm.info.Author = info.Author;//设置算法信息
                        algorithm.info.name = info.Name;//设置算法名
                        algorithm.info.description = info.Introduction;//设置算法介绍
                        algorithm.info.type = type;//设置算法类型
                    }

                    if(Core_Function_Status || Information_Function_Status){//如果算法dll函数有效或信息函数有效
                        Algorithm_Table.push_back(std::move(algorithm));//将算法信息添加到算法函数表
                    }

                }catch(std::runtime_error& e){
                    std::cerr << "[\033[31mAnalysis DLL\033[30m] " << e.what() << "," << e.what() << std::endl;
                }
            }

            return Algorithm_Table;//返回算法函数表

        }




};



        /*
        void Overload_DLLs(std::vector<std::string> Document_path){//加载DLLs
            //清理列表
            make_algorithm_Info.clear();
            find_algorithm_Info.clear();
            make_algorithm.clear();
            find_algorithm.clear();
    
            for(auto& Document : Document_path){
                try{
                    //dylib::library lib(Document);//加载DLL
                    //auto lib = std::unique_ptr<dylib::library>(Document);//加载DLL
                    auto lib = std::make_unique<dylib::library>(Document);
                    auto Plugin_Info = lib->get_function<Information()>("GetInformation");//获取dll算法信息
                    auto init_func = lib->get_function<int(const MazePluginAPI* api)>("plugin_init");//获取dll算法初始化函数
                    auto Plugin_Run = lib->get_function<void()>("Run_plugin");//获取dll算法运行函数
                    auto Final_cleanup = lib->get_function<void()>("Final_cleanup");//获取dll算法清理函数

                    if(Plugin_Info == nullptr || init_func == nullptr || Plugin_Run == nullptr || Final_cleanup == nullptr){
                    //使用异常处理
                    throw std::runtime_error(Document);
                    }

                    Information info = Plugin_Info();//获取dll算法信息

                    if(info.Type != CREATE_ENUM && info.Type != SEARCH_ENUM){//检查算法类型是否合法
                        throw std::runtime_error("Unknown algorithm type in DLL: " + Document); //未知算法类型
                    }

                    //注册算法信息和地址
                    std::vector<Algorithm_Address>& Algorithm_Info = (info.Type == CREATE_ENUM) ? make_algorithm_Info : find_algorithm_Info; //根据算法类型选择算法信息表
                    std::vector<Algorithm_Function>& Algorithm_address = (info.Type == CREATE_ENUM) ? make_algorithm : find_algorithm; //根据算法类型选择算法地址表

                        Algorithm_Info.push_back(
                            Algorithm_Address{
                                info.Name, info.Author, info.Introduction
                            }
                        );
                        Algorithm_address.push_back(
                            Algorithm_Function{
                                std::move(lib), init_func, Plugin_Run, Final_cleanup // 将库对象移动到算法地址表
                            }
                        );

                }catch(std::runtime_error& e){
                    std::cerr << "[\033[31mFailed to load DLL\033[30m] " << e.what() << std::endl;
                }
            }
        }
*/
//        void Run_algorithm(int Algorithm_Number, Algorithm_Type algorithm_Type){
//
//            std::vector<Algorithm_Function>& Algorithm_address = (algorithm_Type == CREATE_ENUM) ? make_algorithm : find_algorithm; //根据算法类型选择算法地址表
//            if(Algorithm_Number < 0 || Algorithm_Number >= Algorithm_address.size()){
//                throw std::runtime_error("Invalid algorithm number"); //算法编号无效
//            }
//            Algorithm_address[Algorithm_Number].Initialize(&interface);//初始化算法
//            Algorithm_address[Algorithm_Number].function();//运行算法
//            Algorithm_address[Algorithm_Number].Clean_up();//清理算法
//        }




class User_actions{//用户操作类
    private://私有成员
    /*data*/
    char getPressedChar(){//获取用户输入的字符
        #ifdef _WIN32
            return _getch();                     // Windows 直接返回
        #else
            struct termios oldt, newt;
            tcgetattr(0, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);   // 关闭规范模式和回显
            tcsetattr(0, TCSANOW, &newt);
            char ch = getchar();                 // 立即读取一个字符
            tcsetattr(0, TCSANOW, &oldt);        // 恢复终端设置
            return ch;
        #endif
    }


    public://用户操作

    bool User_confirms_action(std::string action){//用户确认操作
        std::cout << "[\033[2mPlease confirm\033[0m]" << action << "(y/n)"; //提示用户输入
        std::cout.flush(); //刷新输出缓冲区
        bool Whether_enter = false; //初始化确认标志
        bool Return_Status = false;//返回确认状态
        while(!Whether_enter){ //循环等待用户输入
            char ch = getPressedChar(); //获取用户输入的字符
            Return_Status = (ch == 'y' || ch == 'Y');//判断用户输入是否为确认字符
            Whether_enter = (ch == 'y' || ch == 'Y'|| ch == 'n' || ch == 'N');
        }
        std::cout << "\r" << "\033[2K";//清除输入行
        std::cout << "[" << (Return_Status ? "Choose - > \033[2m\033[32mYES\033[0m" : "Choose - > \033[2m\033[31mNO\033[0m") << "]";
        std::cout << action << std::endl;
        return Return_Status; //返回确认状态
    }



};


#endif // MAZEINFRA