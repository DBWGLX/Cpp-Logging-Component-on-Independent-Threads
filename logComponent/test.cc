#include <iostream>
#include "dbwg.log.h"
#include <time.h>
#include <chrono>
using namespace dbwg;
#include <thread>
#include <vector>


void threadFunc(){
    for(int i=0;i<200000;i++){
        debug_str("模拟日志，日志序号"+std::to_string(i)+"🤓🌹⭐");
    }
}
int main(){
    LOG("程序启动");
    LogStarter::instance();

    auto start = std::chrono::high_resolution_clock::now();
    ////////////////
    
    int n = 5;
    std::vector<std::thread> threads;
    for(int i=0;i<n;i++){
        threads.emplace_back(threadFunc);
    }

    for(auto &thread :threads){
        thread.join();
    }

    ////////////////
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
    return 0;
}


// 多线程同步（用同一个日志器写）
// #include <thread>
// #include <vector>

// #include "SyncLogger.hpp"
// SyncLogger synclogger;

// void threadFunc(){
//     for(int i=0;i<200000;i++){
//         synclogger.log("模拟日志，日志序号"+std::to_string(i)+"🤓🌹⭐",__FILE__,__LINE__,level::DEBUG);
//     }
// }
// int main(){
//     LOG("程序启动");
//     auto start = std::chrono::high_resolution_clock::now();
//     ////////////////
    
//     int n = 5;
//     std::vector<std::thread> threads;
//     for(int i=0;i<n;i++){
//         threads.emplace_back(threadFunc);
//     }

//     for(auto &thread :threads){
//         thread.join();
//     }

//     ////////////////
//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
//     return 0;
// }