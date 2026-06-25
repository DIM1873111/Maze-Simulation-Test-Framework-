#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <thread>

#include "infra.hpp"//包含地图数据解析头文件
#include "SFML_map.hpp"//包含地图数据解析头文件
#include "typeenum.hpp"//包含数据类型头文件


class Receive_map_info{

    private://私有

    int port_Receive = 8889;//接收端口a
    int port_Send = 8888;//发送端口b

    int mapx = 0;//地图x
    int mapy = 0;//地图y

    
    int Render_Size = 20;//渲染大小
    int Delayed_reception_time = 1000;//延迟接收时间
    //int Delayed_data_reception_time = 1000;//延迟接收数据时间
    
    struct Command_List{
        std::string sign;//指令标志
        std::function<void(std::vector<std::string> data)> Command;
    };



    std::string ip = "127.0.0.1";//接收端口ip
    std::string Config_file = "config.json";//配置文件名
    std::string Password;//密码
    std::vector<Command_List> Command_list;//命令列表
    bool Map_status = false;//地图状态
    bool Render_Status = true;
    Communication target;//通信对象
    SFMLGraphics_Tool Graphics;//图形工具
    json_ConfigRelated Config;//配置相关
    String_manipulation string_manipulation;//字符串处理对象

    void Initialize_Settings(){//初始化变量

        std::vector<json_ConfigRelated::config_Address> config1 = {
            {"Render_Port_a", &port_Receive, datatype::Data_type::INT_ENUM},
            {"Render_Port_b", &port_Send, datatype::Data_type::INT_ENUM},
            {"Render_IP", &ip, datatype::Data_type::STRING_ENUM}
        };
        if(Config.Check_file(Config_file)){
            Config.Update_config(config1, Config.Open_json_file(Config_file));
        }else{
            std::cout << "Config file not found, creating new config file" << std::endl;
        }
        for(auto& Variable : config1){
            std::cout << Variable.config_name << " = " << string_manipulation.variantToString(string_manipulation.ToValueVariant(Variable.Address)) << std::endl;
        }

    }

    void Initialize_target(){//初始化通信对象
        target.Create_network_object(ip, Delayed_reception_time, port_Send, port_Receive);
    }




    void Initialize_rendering(std::vector<std::string> Map_settings){//初始化渲染

        if(Map_status != false){target.Send_Single_state("Map_finished_initializing");}//如果地图已经初始化则抛出异常
        if(Map_settings.size() != 1){throw std::runtime_error("Invalid map settings");}//如果地图设置数量不为1则抛出异常
        std::vector<int> map_data = string_manipulation.Convert_string_array_integer(Map_settings);
        mapx = map_data[0];
        mapy = map_data[1];
        
        Graphics.Initialize_window("SFML window with Vulkan", mapx, mapy, Render_Size);//初始化窗口
        std::cout << "Map initialized successfully" << std::endl;
        target.Send_Single_state("Map_finished_initializing");
        Map_status = true;

    }

    public://公有


    void Communication_Test(){
        bool Test_Status = false;
        while(!Test_Status){
            target.Send_Single_state("Test");//发送测试状态
            if(target.Receive_Single_state("Test")){
                Test_Status = true;
                std::cout << "Connection successful" << std::endl; // 连接成功
            }else{
                std::cout << "Connection failed, waiting to retry" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 等待1秒
            }
        }
    }

    void Verify_password(){//验证密码
        bool Verification_Status = false;//验证状态
        while(!Verification_Status){
        target.Send_individual_data("Verify_password", Password);
        Communication::Network_Status Status =  target.Receive_status("Verify_password");
        switch(Status){
            case Communication::Network_Status::GOT_IT_TRUE:
                Verification_Status = true;
                std::cout << "Password verification successful" << std::endl;
            break;
            
            case Communication::Network_Status::GOT_IT_NULL:
                std::cout << "Password verification failed, please try again" << std::endl;
                std::cin >> Password;
            break;

            case Communication::Network_Status::NO_MATCH :
                std::cout << "Server data doesn't match" << std::endl;
            break;

            case Communication::Network_Status::NO_RECEIVE :
                std::cout << "Server data not received" << std::endl;
            break;

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));//等待500毫秒
    }

    }


    void Processing_server_commands(){
        while(Render_Status){
            Communication::Transferdata Status = target.Receive_data();//接收指令
            std::string sign = Status.Sign;
            std::vector<std::string> data = Status.data;
            bool Command_Status = false;//指令状态
            for(auto& Command : Command_list){
                  if(sign == Command.sign){
                      Command.Command(data);//执行指令
                      Command_Status = true;
                      break;
                    }
            }
            if(!Command_Status){
                std::cout << "Command not found" << std::endl;
            }
        }
    }






    Receive_map_info(std::string password) : Password(password){//构造函数
        Initialize_Settings();
        Initialize_target();
        Command_list = {
        {"Initialize_rendering", [this](std::vector<std::string> data){Initialize_rendering(data);}},//初始化指令
        };
    }


};


















int main(int argc, char* argv[]){
    Receive_map_info chat((argc > 1) ? argv[1] : "No_password");
    chat.Communication_Test();//测试连接
    chat.Verify_password();//验证密码
    chat.Processing_server_commands();//处理服务器指令
    return 0;
}

/*
    //创建对象
    Receive_map_info chat;
    std::string password;
    bool Verification_Status = false;//验证状态
    //chat.set_communication(true);//设置通信模式为非阻塞模式
    while(!Verification_Status){

        if(argc > 1){
            password = argv[1];
            argc--;//防止重复输入
        }else{
            std::cout << "Please enter the password:" << std::endl;
            std::cin >> password;
        }

        switch(chat.Send_password(password)){

            case 0:
                std::cout << "Password verification failed" << std::endl;

            break;
            
            case 1:
                std::cout << "Password verification successful" << std::endl;
                Verification_Status = true;
            break;

            case -1:
                for(int i = 0; i < 20; i++){//重试连接
                    int State = chat.Send_password(password);
                    if(State != -1){
                        Verification_Status = (State==1);
                    }
                    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                if(Verification_Status){
                    std::cout << "Password verification successful" << std::endl;
                }else{
                    std::cout << "Connection failed" << std::endl;
                }
            break;

        }
        
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));//等待1毫秒


    }

    if(!Verification_Status){//如果EOF则关闭
        std::cout << "The program crashed and is closing" << std::endl;
        exit(0);
    }

    while(true){//主渲染循环 处理服务器指令数据并回复

        chat.Status_Communication();//状态通信




    }

*/
/*
    //创建对象
    String_manipulation Communication_target;
    SFMLGraphics_Tool Graphics;    
    Map map;

    Communication_target.Initialize(8889, 8888);
    Communication_target.Send_Data("1234");//发送数据

    std::vector<std::string> received_data = Communication_target.Analyze_raw_inputdata(Communication_target.Receive_Data());
    if(received_data.empty() || received_data.size() > 2){
        std::cout << "Invalid received data" << std::endl;
    }
    //如果接收到的数据为空或数量超过1
    
    int x = std::stoi(received_data[0]);
    int y = std::stoi(received_data[1]);
    //创建地图
    map.Set_map_size(x , y);
    Graphics.Initialize_window("SFML window with Vulkan",x , y);//初始化窗口
    Communication_target.Set_receive(true);//设置接收端口为非阻塞模式


    int Render_Count = -1;//渲染计数器
    std::vector<bool> Number_rows(y, false);//初始化行获取状态

    while(true){

        
        Graphics.Handle_window();//处理窗口事件

        std::vector<std::string> received_data = Communication_target.Analyze_raw_inputdata(Communication_target.Receive_Data());
        if(received_data.empty()){//如果接收到的数据为空跳过并等待
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }

        std::string Data_type = received_data[0];

        if(Data_type == "row_data"){//行数据
            int row = std::stoi(received_data[1]);//获取行号
            if(row < 0 || row >= y){
                continue;
            }//如果行号超出范围跳过

            received_data.erase(received_data.begin(), received_data.begin() + 2);
            std::vector<int> int_new_vec;
            
            for(size_t i = 0; i < received_data.size(); i++){
                int_new_vec[i] = std::stoi(received_data[i]);
            }
            
            map.Set_map_data(int_new_vec, row);//设置地图数据
            Number_rows[row] = true;

        }else if(Data_type == "request_refresh"){//刷新请求

            if(received_data.size() != 1 || received_data.size() < 1){
                continue;
            }

            std::vector<int> Missing_row;//缺少的行列表
            for(size_t i = 0;i<Number_rows.size();i++){
                if(!Number_rows[i]){Missing_row.push_back(static_cast<int>(i));}
            }

            if(Missing_row.empty() || std::stoi(received_data[1]) == Render_Count){
                Map_Parsing(map.Get_map_data(), map, Graphics);//解析地图数据
                Graphics.Render_refresh();//刷新渲染
                Communication_target.Send_Data("Refresh_complete");//发送刷新完成
                continue;            
            }

            Communication_target.Send_Data("lacking "+ Communication_target.Processing_data(Missing_row));//发送缺少的行列表
            std::cout << "lacking "+ Communication_target.Processing_data(Missing_row) << std::endl;//输出缺少的行列表

        }else if(Data_type == "close_window"){//关闭窗口
            Communication_target.Send_Data("Done_close_window");
            Graphics.Close_window();
            break; // 退出循环
        }

    }

    return 0;
}*//*

    //接受数据并解析

    void Initialize_graphics(int x , int y){//初始化图形工具
        Graphics.Set_map_size(x , y);//设置地图大小
        Graphics.Initialize_window("SFML window with Vulkan",x , y,Render_Size);//初始化窗口
        Map_status = true;//地图状态
    }

    void Set_map_data(std::vector<int> map_data , int row){
        Graphics.Set_map_data(map_data , row);//设置地图数据
    }

    void Refresh_map(){//刷新地图
        Graphics.Map_Parsing();
        Graphics.Render_refresh();
    }

    void Send_Request(std::string sign){
        target.Send_ID(sign);//发送请求
    }

    void Send_data_request(std::string sign, std::vector<std::string> data){
        target.Send_data_string(sign, data);//发送数据请求
    }

    public:

    void Recognize_server_commands(){//识别服务器指令并处理
        Communication::Transferdata command = target.Delayed_data_reception(Delayed_reception_time);//接收指令
        std::string sign = command.Sign;
        std::vector<std::string> data = command.data;
        if(sign.empty() || data.empty()){
            return;//如果指令为空或数据为空则返回
        }

        if(sign != "Initialize" && Map_status == false){
            target.Send_ID("Please_initialize");//如果指令不是初始化且地图状态为false则发送初始化请求
            return;
        }

        if(sign == "Initialize"){

            std::cout << "Initializing..." << std::endl;
            if(data.size() != 1){
                std::cout << "Invalid data format" << std::endl;
                return;
            }

            mapx = std::stoi(data[0]);
            mapy = std::stoi(data[1]);

            Initialize_graphics(mapx , mapy);//初始化图形工具

        }else if(sign == "row_data"){

            if(data.size() != mapx+1){
                std::cout << "Data doesn't match" << std::endl;
                return;
            }

            int row = std::stoi(data[0]);
            std::vector<int> row_data;

            data.erase(data.begin());//删除row标识

            for(auto& data : data){//遍历数据并转换为int类型
                row_data.push_back(std::stoi(data));
            }


            Set_map_data(row_data , row);//设置地图数据

        }else if(sign == "Request_Refresh"){//刷新请求

            刷新地图数据请求

        }




    }


    int Send_password(std::string password){
        target.Send_Data("Password_verification " + password);
        return target.Delayed_status_reception("Verification_Status", Delayed_data_reception_time);//发送密码并接收状态
    }

    void Status_Communication(){
        if(target.Delayed_status_reception("Test",Delayed_data_reception_time)){
            //std::this_thread::sleep_for(std::chrono::milliseconds(10));//等待

            target.Send_Status("Test", true);
        }

    }


*/
