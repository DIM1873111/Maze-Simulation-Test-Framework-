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



//[Maze2.h框架]

class Mazesimulate {
    private:

        int Max_number_logs = 20;//最大日志数
        int Map_length_X = 10;//地图长度X
        int Map_length_Y = 10;//地图长度Y




        //前向声明嵌套类
        class Log_Exit;//日志或退出处理
        class Input_processing;//用户输入处理
        class Data_Processing;//数据处理
        class Config_Reading;//默认配置读取类(json文件读取)
        class Map_data;//地图数据操作



        class Map_data{//地图数据类

            public:

                enum class Map_type{//地图类型

                    FINISH_ENUM = -2,//终点
                    START_ENUM = -1,//起点

        
                    EMPTY_ENUM = 0,//空
                    WALL_ENUM = 1,//墙
                
                
                };

            private://私有

               std::vector<std::vector<Map_type>> map_data;//地图数据
               Mazesimulate* mazesimulate;//获取Mazesimulate对象

            public://公有

                Map_data(Mazesimulate* p):mazesimulate(p){}//构造函数



                void Initialize_map_data(Map_type Initialization_Type);//初始化地图数据
                

                //地图API接口(待做)

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
            
                std::vector<config_Address> config;//配置表
            
                //void Create_Config();//创建json文件 并且写入默认配置
            
                void Read_config();//从json文件读取配置
                
                bool Check_config_file();//检查配置文件是否存在
            
            public://公有

                Config_Reading(Mazesimulate* p):mazesimulate(p){}//构造函数

                void Add_config(std::string config_name, int* Address);//添加配置

                void Load_Config();//加载配置

                void Create_Config();//创建json文件 并且写入默认配置

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



                void cout_Log(std::string log, Log_type type);//日志函数
                void Exit(std::string exit, Exit_type type);//退出函数

        };


        class Input_processing{//用户输入处理

            private://私有

                Mazesimulate* mazesimulate;//获取mazesimulate对象
                

                //结构体
                struct config_Address{
                std::string config_name = "null";//配置名
                std::string config_description = "null";//配置介绍
                int* Address;//配置地址
                };

                struct User_Input{
                std::string name;//用户输入配置名
                int value;//用户输入值
                };


                std::vector<config_Address> config;//配置信息
                std::vector<User_Input> UserInput;//用户输入信息

                void Matching_config_table();//匹配配置表

            public:


                Input_processing(Mazesimulate* p):mazesimulate(p){}


                void add_config_table(std::string config_name, std::string config_description, int* Address);//添加配置表

                void Get_Input();//获取用户输入

                //void Parameter_Start();//参数启动

        };

        class Data_Processing{//数据处理


            private:


                Mazesimulate* mazesimulate;//获取mazesimulate对象


            public:


                Data_Processing(Mazesimulate* p):mazesimulate(p){}

                void Add_config_table();//快速添加配置

                void Initialize_data();//初始化数据


        };




        //成员对象构造时传入 this

        Map_data Map_class{this};//地图数据对象
        Config_Reading Config_class{this};//配置读取对象
        Log_Exit Log_class{this};//日志处理对象
        Input_processing Input_class{this};//用户输入处理对象
        Data_Processing Data_class{this};//数据处理对象



    public://公有

        void Framework_Entry();//程序入口函数




};



#endif // MAZEDIM_H
