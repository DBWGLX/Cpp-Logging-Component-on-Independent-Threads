//#pragma once
#include <iostream>
#include "dbwg.log.h"
#include<time.h>
using namespace dbwg;


int main(){
    LOG("程序启动");
    int count = 1;
    clock_t ct = clock();

    while(count <= 100){
        //debug_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        fatal("haha %d",count);
    }
    
    ct = clock() -ct;
    std::cout << ct << std::endl;
    return 0;
}


