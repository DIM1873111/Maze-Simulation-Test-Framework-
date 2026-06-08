#ifndef MAZEDIM_H
#define MAZEDIM_H

#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <unordered_map>


class Mazesimulate{

    private://私有

        int Max_log = 20;//Log_Exit类最大日志数

        //key
        friend class Input_processing;//用户输入处理类
        friend class Data_Processing;//数据处理类
        friend class Log_Exit;//日志或退出类



    
        class Log_Exit{//日志或退出

            private://私有成员

                Mazesimulate *mazesimulate;//获取Mazekye对象

                std::vector<std::string> Log_container;//日志容器

            public://公有成员

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

                };
                void cout_Log(std::string log, Log_type type);//日志函数

                void Exit(std::string exit, Exit_type type);//退出函数

                std::vector<std::string> Output_historical_logs();//输出历史日志函数
        };



        Log_Exit Log_class;//创建日志或退出对象



        class Input_processing{//用户输入处理

            private://私有成员

            //结构体
            struct config_Address{
            std::string config_name;//配置名
            int* Address;//配置地址
            };

            struct User_Input{
            std::string name;//用户输入名
            int value;//用户输入值
            };


            std::vector<config_Address> config;//配置信息
            std::vector<User_Input> UserInput;//用户输入信息

            Log_Exit *Log_class;//获取Log_class对象
            Mazesimulate *mazesimulate;//获取Mazekye对象

            public://公有成员



            void Get_Input();//用户输入处理函数


        };




        class Data_Processing{//数据处理

            private://私有成员

            Input_processing Input_class;//创建用户输入处理对象

            public://公有成员

            void Initialize_data();//数据处理函数

        };








    public://公有



        void Framework_Entry();//入口








        //Mazesimulate();
        //~Mazesimulate();
};


#endif // MAZEDIM_H
