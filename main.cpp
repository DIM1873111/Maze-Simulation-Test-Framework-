#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "maze2.h"

void randomGenerate(Mazesimulate& map){

    /*random generate function*/
    Mazesimulate::location Map_size = map.mapapi.Get_mapsize();//测试API
    std::cout << "Map size: " << Map_size.x << "x" << Map_size.y << std::endl;
    map.mapapi.Set_allmaptype(Mazesimulate::Map_type::EMPTY_ENUM);//测试全图
    map.mapapi.Set_maptype(0,0, Mazesimulate::Map_type::WALL_ENUM);//测试标记

    map.mapapi.Output_log("I am a test log. This algorithm is used for testing. QWQ", Mazesimulate::log_type::INITIAL_ENUM);//测试日志
    //throw std::runtime_error("Test message =)");//测试异常

}


int main(int argc, char* argv[]){

    /*main function*/


    Mazesimulate mazeSimulator;
    mazeSimulator.add_make_Method(//测试
        "randomGenerate",
        "random generate function",
        randomGenerate
    );
    mazeSimulator.Framework_Entry();


    return 0;
}