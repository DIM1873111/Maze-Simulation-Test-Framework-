#ifndef TYPEENUM_HPP
#define TYPEENUM_HPP


class maptype{
    private:
    /*data*/
    //protected://保护类
        public://公有类

        enum class Map_type{//地图数据类型

            FINISH_ENUM = -2,//终点
            START_ENUM = -1,//起点

        
            EMPTY_ENUM = 0,//空
            WALL_ENUM = 1,//墙
        

            SEARCH_ENUM = 2,//搜索
            SCAN_ENUM = 3,//搜索(扫描)

            ROUTE_ENUM = 4,//路线

        };


};

class logtype{
    private:
    /*data*/
    //protected://保护类
        public://公有类

        enum class Log_type{//日志类型

           PROMPT_ENUM,//提示日志
           NOTICE_ENUM,//通知日志
           WARNING_ENUM,//警告日志
           ERROR_ENUM,//错误日志
                           
        };

        enum class Exit_type{//退出类型

           STANDARD_ENUM,//标准退出
           OPERATIONAL_ENUM,//操作退出
           //ERROR_ENUM,//错误退出(算法错误)
           SYSTEM_ENUM,//系统退出

        };

        enum class Algorithm_Log{//算法日志类型

            NORMAL_ENUM,//通知日志
            ERROR_ENUM,//错误日志
            PROCESS_ENUM,//处理日志(算法处理流程)
            INITIAL_ENUM,//初始化日志(数据初始化)
            
        };


};

class datatype{
    private:
    /*data*/
    protected://保护类
        enum class Data_type{
            INT_ENUM,//整数枚举
            FLOAT_ENUM,//浮点数枚举
            BOOL_ENUM,//布尔枚举
            STRING_ENUM,//字符串枚举
            NULL_ENUM//未知
        };

};







#endif
