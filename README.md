# 项目大概介绍
- 项目名字 [Dim|C++]mazeAI 一个简单的控制台迷宫模拟的C++项目
- 目前没有图形化界面 不过操作起来还是很简单的
- 整个项目使用控制台显示渲染完成
- 感兴趣的可以使用这个项目来练习一下算法实现 扩展挺方便的
- 这是我作为初学者自学39天完成的第一个一个小项目()
- 有些地方可能有问题 毕竟我没学多久 后面会尝试优化的
- 顺带一提 你可能会有速度更快的的算法 想象力更丰富自定义的算法 但是可能需要一个框架 那么来试试这个项目吧 他给你提供了比较完整的算法实现框架 可以自定义实现
# 文件用途 以及如何扩展
- [batc.bat]编译脚本(win平台的) 也可以直接运行"g++ maze_ai_main.cpp maze_ai_hs.cpp Generation_algorithm_lib.cpp Search_algorithm_lib.cpp -o mazeAI_byDim"指令编译
- [Maze_ai_lx.h]头文件
- [maze_ai_main.cpp]启动文件 调用函数的 **一般来说不要改*
- [maze_ai_hs.cpp]主函数库 实现动态渲染 初始化数据之类的 还有错误处理 调用算法
- [Search_algorithm_lib.cpp]搜索算法库 里面一般来说都是放的路线搜索算法
- [Generation_algorithm_lib.cpp]这个文件是生成迷宫的算法仓库 里面的函数主要是生成迷宫的 

在[maze_ai_hs.cpp]文件里面 如果你添加了新的算法 可以注册到算法库里面
```cpp
void Maze_AI::Load_algorithm_library(){//初始化算法库
Algorithm_Library_Create.push_back({"生成算法名字","介绍",std::bind(&Maze_AI::你的算法函数名, this,类型)});
Algorithm_Library_Search.push_back({"搜索算法名字","介绍",std::bind(&Maze_AI::你的算法函数名, this,类型)});
}
```
**不要忘记头文件添加()*
弄完了启动之后可以使用提示输入你的算法编号
在maze_ai_hs里面错误处理可以直接使用exits("错误信息")来进行退出处理
- 如果需要制作算法 
可以使用Map_loading("当前状态")来进行渲染地图画面
模拟动画效果
其他的可以参考一下直带的算法函数
# 自带算法
- DFS，BFS(创造)，双向DFS，A，双向A DP动态规划(真/假)
- 你也可以删除使用你自己的算法 或者参考参考