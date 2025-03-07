//#pragma once
#include <iostream>
#include "dbwg.log.h"
#include<time.h>
using namespace dbwg;
#include <chrono>

int main(){
    LOG("程序启动");
    int count = 1;

    LogStarter::instance();
    auto start = std::chrono::high_resolution_clock::now();

    while(count <= 1000000){
        debug_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        info_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        warn_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        error_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        fatal_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        debug("模拟日志，日志序号%d🤓🌹⭐",count++);
        info("模拟日志，日志序号%d🤓🌹⭐",count++);
        warn("模拟日志，日志序号%d🤓🌹⭐",count++);
        error("模拟日志，日志序号%d🤓🌹⭐",count++);
        fatal("模拟日志，日志序号%d🤓🌹⭐",count++);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
    return 0;
}


