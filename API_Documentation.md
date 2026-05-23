# 地图操作

## 地图枚举
    **标记类**
- Maze_Destination=-1,//终点
- Maze_Starting_point=-2,//起点
- Empty_data_Enum=-3,
- Gold_Coin_Special=-4,//金币特殊标记
- Gold_Coin_Tag=-5,//扫描金币标记
- Scan_Mark=-6,//扫描标记
    **实体类**
- Maze_walls_Enum=0,//墙壁    
- Maze_empty_Enum=1,//空
- Maze_aipath_Enum=2,//寻找痕迹
- Maze_Searching_Traces1=3,//寻找痕迹1
- Maze_Searching_Traces2=4,//寻找痕迹2
- Maze_Shortest_route=5,//最短路线标记

## 操作数据检查
- Boundary_check(int x,int y);//检查边界是否合法
- Check_index(int Index);//检查索引是否合法
- Check_Destination(int x,int y);//检查当前位置是否终点
- Check_starting_point(int x,int y );//检查当前位置是否起点
- Tag_Information(int x,int y,int Type);//检查特定的标记是否符合要求
**注意：以上函数均返回布尔值，true表示合法，false表示不合法**
## 地图数据初始化
- Set_destination(int Index);//设置地图终点
- Set_Starting_point(int Index);//设置地图起点
**以上函数在创造地图时调用，设置起点和终点**
## 地图数据操作
- Mark_Trace(int x,int y,int Type);//设置地图数据标记(x位置，y位置，标记的类型)
## 地图数据获取
- Obtain_destination();//获取地图终点位置(引索)
- Obtain_Starting_point();//获取地图起点位置(引索)
- Get_indexX(int Index);//计算引索对应的X坐标
- Get_indexY(int Index);//计算引索对应的Y坐标
## 其他

Random_number(int min,int max)//产生随机数
Take_the_maximum(int a,int b)//求最大值
Take_the_minimum(int a,int b)//求最小值
- add_Create(std::string Name,std::string Introduction,std::function<void()> Run,int Type);//添加创造库
- add_Search(std::string Name,std::string Introduction,std::function<void()> Run,int Type);//添加搜索库
- Map_loading(std::string mapdata);//刷新地图
- clean_animation();//清屏




