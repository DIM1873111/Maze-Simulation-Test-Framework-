# 项目基础信息介绍
- 项目名字 [Dim|C++]mazeAI 一个简单的控制台+图形化迷宫模拟的C++项目
- 项目提供一个迷宫框架 基于控制台字符串显示+SFML图形化 的迷宫模拟 可以模拟生成迷宫以及搜索迷宫 并且可视化过程
- 以及大部分必要的api接口(函数) 详细可以查看文档[API_Documentation.md]
- zhangsan 20260427(项目开始日期)
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
- 或者直接在命令行里运行
- g++ -std=c++17 -I"C:\Users\29915\Desktop\SFML-3.1.0\include" -L"C:\Users\29915\Desktop\SFML-3.1.0\lib" main.cpp maze.cpp Create.cpp Search.cpp -o maze_program -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lgdi32 -lwinmm
-实际运行替换掉你的SFML路径
2. 模拟
- 直接打开生成的程序文件 输入 地图x 地图x 生成算法 寻路算法 即可开始模拟
- 或者提供命令行参数 [程序名] [地图x] [地图y] [生成算法] [寻路算法] 直接运行程序
**运行时可以控制速度以及暂停**
- + 加快运行速度
- - 减少运行速度
- Space 暂停
- ESC 退出程序
3. 算法选择
- 目前算法实现以及规划
- 1.DFS 深度优先搜索 [已实现]
- 2.BFS 广度优先搜索 [已实现]
- 3.A* 启发式搜索 [已实现/多个版本]
- 4.Dijkstra 迪克斯特拉算法 [已经实现/多个版本]
- 5.Greedy 贪婪算法 [部分复合算法实现]
- 6.Hill Climbing 爬山算法 [待实现]
- 7.Random 随机算法 [待实现]
- 8.Recursive Backtracking 递归回溯算法 [待实现]
- 9.Iterative Deepening 迭代深度搜索 [待实现]
- 10.Simulated Annealing 模拟退火算法 [待实现]
- 11.Genetic Algorithm 遗传算法 [待实现]
- 12.MST Minimum Spanning Tree 最小生成树算法 [待实现]
- 13.Kruskal 克鲁斯卡尔算法 [待实现]
- 14.Prim 普里姆算法 [已实现]
- 15.Bellman-Ford 贝尔曼-福特算法 [待实现]
- 16.Floyd Warshall 弗洛伊德算法 [待实现]
- 17.Johnson 约翰内斯算法 [待实现]
- ...【待补充】