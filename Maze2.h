#ifndef MAZEDIM_H
#define MAZEDIM_H

#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <unordered_map>
#include <queue>
#include <variant>


#include "typeenum.hpp"//包含类型枚举
#include "infra.hpp"//包含基础设施头文件

//第三方库
#include "third_party\kissnet.hpp"
#include "third_party\json.hpp"
#include "third_party\process\process.hpp"

//Mazesimulate作为Maze_AI的第2版 这一版本打算重构框架结构 并且添加一些新的功能
//原项目(老版本)完整项目链接:https://github.com/DIM1873111/Maze-Simulation-Test-Framework-
//maze2开始日期2026年6月7日
//迷宫模拟平台框架(.HPP)

//继承MazeInfra


class Mazesimulate{

    private://私有

        //前向声明嵌套类
        class Log_Exit;//日志或退出处理
        class Config_Reading;//默认配置读取类(json文件读取)        
        class Input_processing;//用户输入处理
        class Data_Processing;//数据处理
        class Render_Send;//渲染发送类
        class Map_data;//地图数据操作
        class Algorithm_Tools;//算法工具类

        int Render_Port_A = 8080;//渲染端口a
        int Render_Port_B = 8081;//渲染端口b
        std::string Render_IP = "127.0.0.1";//渲染IP地址

        int Render_Password = 9999;//渲染口令
        
        int Max_number_logs = 20;//最大日志数
        int Calculation_interval = 100;//计算间隔(毫秒)


        int Map_length_X = 10;//地图长度X
        int Map_length_Y = 10;//地图长度Y

        int Map_Max_area = 500*500;//地图最大面积

        int Map_Min_X = 5;//地图最小长度X
        int Map_Min_Y = 5;//地图最小长度Y

        int Selected_generation_method = 0;//选择生成方法
        int Selected_search_method = 0;//选择搜索方法

        class Log_Exit : public logtype{//日志或退出处理(继承logtype类)


            private://私有

                Mazesimulate* mazesimulate;//获取Mazesimulate对象
                std::queue<std::string> historical_logs;//历史日志队列

            public:

                Log_Exit(Mazesimulate* p):mazesimulate(p){}//构造函数

                void cout_Log_system(std::string log, Log_type type);//输出系统日志

                void record_Log_Algorithm(std::string log, Algorithm_Log type);//记录算法日志

                void Export_history_log();//导出容器日志

                void Exit(std::string exit, Exit_type type);//退出函数

        };


        class Config_Reading : public json_ConfigRelated {//默认配置读取类(json文件读取)

            private://私有

                Mazesimulate* mazesimulate;
                
                std::string configPath = "config.json";//配置文件名
            
                std::vector<config_Address> config;//配置表

                void Add_config_table();//初始化配置表

            public://公有

                Config_Reading(Mazesimulate* p):mazesimulate(p){//构造函数
                    Add_config_table();
                    Load_Config();
                }

                

                void Load_Config();//加载配置

                void Create_Config();//创建json文件 并且写入当前配置(第一次运行时调用默认配置)

                //void Generate_port_file();//生成渲染端口配置文件
        };


        class Map_data : public maptype{//地图数据类

            public:

                enum class Map_Status{//状态标签

                    NOT_INITIALIZED_ENUM,//未初始化
                    INITIALIZED_ENUM,//已创建
                    GENERATED_ENUM,//已生成

                };

                struct Map_Info{//地图信息结构体

                    int practical_X = 0;//地图长度X
                    int practical_Y = 0;//地图长度Y
                    int map_area = 0;//地图面积
                    Map_Status map_status = Map_Status::NOT_INITIALIZED_ENUM;//地图状态

                };

                struct Location{//位置结构体
                    int x;//X坐标
                    int y;//Y坐标
                };

            private://私有

                Map_Info map_info;//创建地图信息

               std::vector<std::vector<Map_type>> map_data;//地图数据
               Mazesimulate* mazesimulate;//获取Mazesimulate对象
               Location start_position;//起点位置(first:X,second:Y)
               Location finish_position;//终点位置

            public://公有

                Map_data(Mazesimulate* p):mazesimulate(p){}//构造函数



                void Initialize_map_data();//创建实际地图数据
                
                void Set_map_alltype(Map_type type);//设置地图类型

                void add_map_status(Map_Status map_status);//设置地图状态

                Map_Info get_map_info();//获取地图信息

                std::vector<std::vector<Map_type>> Get_map_data();//获取地图数据(复制一份)
                
                //地图API接口

                bool Check_location_range(int x, int y);//检查地图位置是否有效

                bool Check_a_tag(int x, int y , Map_type a_tag);//检查地图某一个位置是否符合某个标签
                
                bool Check_the_finish(int x, int y);//检查地图是否到达终点

                bool Check_the_start(int x, int y);//检查地图是否到达起点

                


                Map_type Get_location_tag(int x, int y);//获取地图位置标签数据

                Location Get_finish_position();//获取终点位置

                Location Get_start_position();//获取起点位置

                Location Get_mapsize();//获取地图大小


                void Set_location_tag(int x, int y , Map_type a_tag);//设置地图某一个位置标签数据

                void Set_finish_position(int x, int y);//设置终点

                void Set_start_position(int x, int y);//设置起点

                
        };

        class Render_Send{//渲染发送类
            /*本类用于渲染数据发送到渲染端口 待实现*/
            private://私有

               Mazesimulate* mazesimulate;//获取Mazesimulate对象
            
                void Settings_Render_Password();//设置渲染口令

            public://公有

            Render_Send(Mazesimulate* p):mazesimulate(p){
                Settings_Render_Password();
                //mazesimulate -> Config_class.Generate_port_file();
            }//构造函数




        };

        class Input_processing : public String_manipulation{//用户输入处理

            private://私有

                Mazesimulate* mazesimulate;//获取mazesimulate对象
                
                

                char delimiter = '.';//分隔符

                //结构体
                struct config_Address{//配置表
                    
                    std::string config_name = "null";//配置名
                    std::string config_description = "null";//配置介绍
                    std::variant<int*, float*, std::string*, bool*> Address;//配置地址
                    Data_type data_type = Data_type::NULL_ENUM;//数据类型
                };

                struct Parameter_Table{//参数表

                    std::string name;//参数名
                    std::string Guide;//介绍
                    std::function<void()> Function;//参数函数
                    
                };

                struct Go_back{
                    bool Find_the_mood;//找到状态
                    int Location;//找到位置
                };


                std::vector<config_Address> config;//配置信息
                std::vector<Parameter_Table> Parameter_table;//参数表(操作)

                void Processing_input(std::string Input);//处理用户输入数据
                
                //std::vector<std::string> Analyze_raw_inputdata(std::string Input);//解析原始用户输入数据

                void Matching_config_table(std::pair<std::string, std::string> Input);//匹配配置表
                
                //void Matching_Table(std::string Input);//尝试匹配配置表

                Go_back Matching_Parameter(std::string Input);//匹配参数并返回操作状态


                void Create_table();//创建表(输入初始化)


                

               //操作函数(用户输入操作)
                void help_operate();

                void Resetjson_operation();

                void List_algorithms();//列出所有算法

                void Version_Info();//版本信息

                void Check_mapinfo();//检查地图信息

            public:


                Input_processing(Mazesimulate* p):mazesimulate(p){//注册操作函数
                Create_table();//创建表
                }


                //void add_config_table(std::string config_name, std::string config_description, int* Address);//添加配置表

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

            public:


                Data_Processing(Mazesimulate* p):mazesimulate(p){}

                //void Add_config_table();//快速添加配置


                void Statistics_Before();//统计前处理

                void Statistics_After();//统计后处理



                void Initialize_data();//初始化数据


        };


        class Algorithm_Tools{//算法工具类
            
            public:
                 struct Algorithm_Address{//算法表
                    std::string name;//算法名
                    std::string description;//算法介绍
                };

                std::vector<Algorithm_Address> make_algorithm_Info;//生成算法地址信息表
                std::vector<Algorithm_Address> find_algorithm_Info;//生成搜索算法信息表

            private://私有

                Mazesimulate* mazesimulate;//获取Mazesimulate对象

                std::vector<std::function<void(Mazesimulate&)>> make_algorithm;//生成算法地址表

                std::vector<std::function<void(Mazesimulate&)>> find_algorithm;//搜索算法地址表


            public://公有


                Algorithm_Tools(Mazesimulate* p):mazesimulate(p){} //构造函数

                void Add_make_algorithm(std::string name, std::string description, std::function<void(Mazesimulate&)> Function_address);//添加算法

                void Add_find_algorithm(std::string name, std::string description, std::function<void(Mazesimulate&)> Function_address);//添加算法

                void Run_make_algorithm(int index);//运行生成算法

                void Run_find_algorithm(int index);//运行搜索算法

        };

        class map_api{//地图API接口

            private://私有

            Mazesimulate* mazesimulate;//获取Mazesimulate对象

            public:

            map_api(Mazesimulate* p):mazesimulate(p){} //构造函数
            
            bool Check_mapscope(int x, int y);//检查xy范围
            bool Check_Maptype(int x, int y, Mazesimulate::Map_data::Map_type a_tag);//检查地图类型
            bool Check_Mapfinish(int x, int y);//检查地图是否到达终点
            bool Check_Mapstart(int x, int y);//检查地图是否到达起点

            Mazesimulate::Map_data::Map_type Get_maptype(int x, int y);//获取地图类型
            Mazesimulate::Map_data::Location Get_mapfinish();//获取终点位置
            Mazesimulate::Map_data::Location Get_mapstart();//获取起点位置
            Mazesimulate::Map_data::Location Get_mapsize();//获取地图大小

            void Set_maptype(int x, int y, Mazesimulate::Map_data::Map_type a_tag);//设置地图类型
            void Set_mapfinish(int x, int y);//设置终点
            void Set_mapstart(int x, int y);//设置起点

            void Set_allmaptype(Mazesimulate::Map_data::Map_type a_tag);//设置全部地图类型

            void Output_log(std::string log, Mazesimulate::Log_Exit::Algorithm_Log type);

        };

        //成员对象构造时传入 this
        Log_Exit Log_class{this};//日志处理对象
        Config_Reading Config_class{this};//配置读取对象               
        Input_processing Input_class{this};//用户输入处理对象 
        Data_Processing Data_class{this};//数据处理对象
        Render_Send Render_send{this};//渲染发送对象
        Map_data Map_class{this};//地图数据对象
        Algorithm_Tools Algorithm_class{this};//算法工具对象


    public://公有

        map_api mapapi{this};//创建地图API接口实例

        using log_type = Mazesimulate::Log_Exit::Algorithm_Log;//日志类型

        using Map_type = Mazesimulate::Map_data::Map_type;//地图数据类型

        using location = Mazesimulate::Map_data::Location;//位置结构体

        

        void Framework_Entry();//程序入口函数

        void add_make_Method(std::string name, std::string description, std::function<void(Mazesimulate&)> Function_address);//添加生成算法
        
        void add_find_Method(std::string name, std::string description, std::function<void(Mazesimulate&)> Function_address);//添加搜索算法


};



#endif // MAZEDIM_H
