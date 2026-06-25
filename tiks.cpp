#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <thread>

#include "infra.hpp"//包含地图数据解析头文件
#include "SFML_map.hpp"//包含地图数据解析头文件
#include "typeenum.hpp"//包含数据类型头文件


Communication target;//通信对象
String_manipulation string_manipulation;//字符串处理对象

void Initialize_communication_object(){
    target.Create_network_object("127.0.0.1", 1000, 8080, 8081);
}

void Reply_test_status(){

    if(target.Receive_Single_state("Test")){
        target.Send_Single_state("Test");//回复测试状态
        std::cout << "Test status received and replied." << std::endl;
    }




}






int main(){
Initialize_communication_object();
while(true){
    Reply_test_status();
}



}
