//#pragma once
#include <iostream>
#include "dbwg.log.h"
using namespace dbwg;


int main(){
    LOG("程序启动");
    int count = 1;
    while(1){
        LOG1("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
    }
    return 0;
}