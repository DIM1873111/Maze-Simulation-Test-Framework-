#ifndef MAZEDIM_H
#define MAZEDIM_H

#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <unordered_map>
#include <queue>


#include "json.hpp"//包含json库

//Mazesimulate作为Maze_AI的第2版 这一版本打算重构框架结构 并且添加一些新的功能
//原项目(老版本)完整项目链接:https://github.com/DIM1873111/Maze-Simulation-Test-Framework-
//maze2开始日期2026年6月7日
//迷宫模拟平台框架(.HPP)

/*

 ____ ____ _________ _________ _________
|    |    |         |         |         |
|         |    |    |______   |    _____|
|  |   |  |    |    |         |         |
|  |   |  |         |   ______|    _____|
|  |   |  |    |    |         |         |
|__|___|__|____|____|_________|_________|
[Maze2.h框架]


*/


class Mazesimulate {
    private:
        int Render_Port = 8080;//渲染端口
        int Render_Password = 9999;//渲染口令
        


        int Max_number_logs = 20;//最大日志数
        int Calculation_interval = 100;//计算间隔(毫秒)


        int Map_length_X = 10;//地图长度X
        int Map_length_Y = 10;//地图长度Y




        //前向声明嵌套类
        class Log_Exit;//日志或退出处理
        class Input_processing;//用户输入处理
        class Data_Processing;//数据处理
        class Config_Reading;//默认配置读取类(json文件读取)
        class Map_data;//地图数据操作
        class Render_Send;//渲染发送类




        class Map_data{//地图数据类

            public:

                enum class Map_type{//地图类型

                    FINISH_ENUM = -2,//终点
                    START_ENUM = -1,//起点

        
                    EMPTY_ENUM = 0,//空
                    WALL_ENUM = 1,//墙
                

                    SEARCH_ENUM = 2,//搜索
                    SCAN_ENUM = 3,//搜索(扫描)

                    ROUTE_ENUM = 4,//路线

                };

                struct Location{//位置结构体
                    int x;//X坐标
                    int y;//Y坐标
                };

            private://私有

               std::vector<std::vector<Map_type>> map_data;//地图数据
               Mazesimulate* mazesimulate;//获取Mazesimulate对象
               Location start_position;//起点位置(first:X,second:Y)
               Location finish_position;//终点位置

            public://公有

                Map_data(Mazesimulate* p):mazesimulate(p){}//构造函数



                void Initialize_map_data(Map_type Initialization_Type);//初始化地图数据
                

                //地图API接口


                bool Check_location_range(int x, int y);//检查地图位置是否有效

                bool Check_a_tag(int x, int y , Map_type a_tag);//检查地图某一个位置是否符合某个标签
                
                bool Check_the_finish(int x, int y);//检查地图是否到达终点

                bool Check_the_start(int x, int y);//检查地图是否到达起点




                Map_type Get_location_tag(int x, int y);//获取地图位置标签数据


                void Set_location_tag(int x, int y , Map_type a_tag);//设置地图某一个位置标签数据

                void Set_finish_position(int x, int y);//设置终点

                void Set_start_position(int x, int y);//设置起点


                std::vector<std::vector<Map_type>> Get_map_data();//获取地图数据(复制一份)
        };



        class Config_Reading{//默认配置读取类(json文件读取)

            private://私有

                Mazesimulate* mazesimulate;
                
                struct config_Address{//配置表
                    std::string config_name;//配置名
                    int* Address;//配置地址
                };
            
                std::string configPath = "config.json";//配置文件名
                std::string configPort = "port.json";//配置渲染端口文件名
            
                std::vector<config_Address> config;//配置表
            
                //void Create_Config();//创建json文件 并且写入默认配置
            
                void Read_config();//从json文件读取配置
                
                

                bool Check_file(std::string File_name);//检查文件是否存在
            


            public://公有

                Config_Reading(Mazesimulate* p):mazesimulate(p){}//构造函数

                void Add_config(std::string config_name, int* Address);//添加配置

                void Load_Config();//加载配置

                void Create_Config();//创建json文件 并且写入默认配置

                void Generate_port_file();//生成渲染端口配置文件
        };


        class Log_Exit{//日志或退出处理


            private://私有

                Mazesimulate* mazesimulate;//获取Mazesimulate对象
                std::queue<std::string> historical_logs;//历史日志队列

            public:

                Log_Exit(Mazesimulate* p):mazesimulate(p){}//构造函数


                enum class Log_type{//日志类型

                   PROMPT_ENUM,//提示日志
                   NOTICE_ENUM,//通知日志
                   WARNING_ENUM,//警告日志
                   ERROR_ENUM,//错误日志
                                   
                };

                enum class Exit_type{//退出类型

                   STANDARD_ENUM,//标准退出
                   OPERATIONAL_ENUM,//操作退出
                   ERROR_ENUM,//错误退出
                   SYSTEM_ENUM,//系统退出

                };

                enum class Algorithm_Log{//算法日志类型

                    NORMAL_ENUM,//通知日志
                    ERROR_ENUM,//错误日志
                    PROCESS_ENUM,//处理日志(算法处理流程)
                    INITIAL_ENUM,//初始化日志(数据初始化)
                    
                };

                void cout_Log_system(std::string log, Log_type type);//输出系统日志

                void record_Log_Algorithm(std::string log, Algorithm_Log type);//记录算法日志

                void Exit(std::string exit, Exit_type type);//退出函数

        };


        class Input_processing{//用户输入处理

            private://私有

                Mazesimulate* mazesimulate;//获取mazesimulate对象
                

                //结构体
                struct config_Address{//配置表
                    
                    std::string config_name = "null";//配置名
                    std::string config_description = "null";//配置介绍
                    int* Address;//配置地址
                
                };

                struct User_Input{//用户输入表

                    std::string name;//用户输入配置名
                    int value;//用户输入值

                };

                struct Parameter_Table{//参数表

                    std::string name;//参数名
                    std::string Guide;//介绍
                    std::function<void()> Function;//参数函数
                    bool is_end_input;//是否结束输入
                };

                enum class Parameter{//参数处理

                    NOT_FOUND_ENUM,//未找到(给配置函数传入)
                    MATCHING_ENUM,//匹配(操作)
                    END_INPUT_ENUM,//结束输入

                };



                std::vector<config_Address> config;//配置信息
                std::vector<User_Input> UserInput;//用户输入信息
                std::vector<Parameter_Table> Parameter_table;//参数表


                void Matching_config_table();//匹配配置表

                Parameter Matching_Parameter(std::string Input);//匹配参数并返回操作状态

                User_Input Transformation_Config(std::string Input);//转换用户输入成配表格格式

                //操作函数(用户输入操作)
                void help_operate();
                void Resetjson_operation();
                void Reset_jsonport_operation();


            public:


                Input_processing(Mazesimulate* p):mazesimulate(p){
                    Parameter_table.push_back({"-help","Help with operations", [this](){help_operate();}, false});//添加默认参数
                    Parameter_table.push_back({"-g","Start the simulation",[](){}, true});//添加默认开始参数
                    Parameter_table.push_back({"-exit","Exit the simulation", [this](){mazesimulate->Log_class.Exit("Simulation exited", Log_Exit::Exit_type::OPERATIONAL_ENUM);}, true});//添加默认退出参数
                    Parameter_table.push_back({"-resetjson","Reset the json file", [this](){Resetjson_operation();}, true});//添加默认重置json参数
                    Parameter_table.push_back({"-resetjsonport","Refresh port configuration file", [this](){Reset_jsonport_operation();}, true});//添加默认重置json端口参数
                }


                void add_config_table(std::string config_name, std::string config_description, int* Address);//添加配置表

                void Get_Input();//获取用户输入

                //void Parameter_Start();//参数启动

        };

        class Data_Processing{//数据处理


            private:


                Mazesimulate* mazesimulate;//获取mazesimulate对象

                int Map_area = 0;//地图面积
                int Map_route = 0;//路线格数量
                int Search_Route = 0;//搜索路线格长度
                int Map_route_proportion  = 0;//原始路线格占比地图(%)
                int Search_Route_proportion  = 0;//搜索路线格占比Map_route_proportion(%)

                int Stats_route_data(Mazesimulate::Map_data::Map_type map_type);//统计路线地图特定数据数量

                void Statistics_Before();//统计前处理

                void Statistics_After();//统计后处理

            public:


                Data_Processing(Mazesimulate* p):mazesimulate(p){}

                void Add_config_table();//快速添加配置

                void Initialize_data();//初始化数据


        };


        class Render_Send{//渲染发送类

            private://私有

               Mazesimulate* mazesimulate;//获取Mazesimulate对象
            
                void Settings_Render_Password();//设置渲染口令

            public://公有

            Render_Send(Mazesimulate* p):mazesimulate(p){
                Settings_Render_Password();
            }//构造函数




        };


        //成员对象构造时传入 this

        Render_Send Render_send{this};//渲染发送对象
        Map_data Map_class{this};//地图数据对象
        Config_Reading Config_class{this};//配置读取对象
        Log_Exit Log_class{this};//日志处理对象
        Input_processing Input_class{this};//用户输入处理对象
        Data_Processing Data_class{this};//数据处理对象



    public://公有

        void Framework_Entry();//程序入口函数




};



#endif // MAZEDIM_H
