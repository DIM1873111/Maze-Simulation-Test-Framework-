# 项目基础信息介绍
- 项目名字 [Dim|C++]mazeAI 一个简单的控制台迷宫模拟的C++项目
- 项目提供一个迷宫框架 基于控制台字符串显示的迷宫模拟 可以模拟生成迷宫以及搜索迷宫 并且可视化过程
- 以及大部分必要的api接口(函数) 详细可以查看文档[API_Documentation.md]
- zhangsan 20260427
# 文件介绍
1. 框架文件
- main.cpp 主函数入口(调用函数)
- maze.cpp 程序主框架实现部分
- maze.h 程序主框架头文件
2. 算法文件
- Create.cpp 迷宫生成算法实现
- Search.cpp 迷宫搜索算法实现
- Algorithm_Repository_v1.h 算法实现头文件
3. 其他
- API_Documentation.md 项目API文档
- README.md 项目介绍文件
- build.bat 编译脚本
# 项目使用
1. 编译
- windows环境下双击build.bat文件即可编译
- 或者直接在命令行里运行g++ -std=c++11 main.cpp maze.cpp Create.cpp Search.cpp -o maze_program
2. 模拟
- 直接打开生成的程序文件 输入 地图x 地图x 生成算法 寻路算法 即可开始模拟
- 或者提供命令行参数 [程序名] [地图x] [地图y] [生成算法] [寻路算法] 直接运行程序