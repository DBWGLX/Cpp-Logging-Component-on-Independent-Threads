#include <iostream>
#include "/root/work/EpollChatServer/cmake_out/include/dbwg/dbwg_logger.hpp"
#include <time.h>
#include <chrono>
using namespace dbwg;
#include <thread>
#include <vector>

std::chrono::high_resolution_clock::time_point start;

// #include "SyncLogger.hpp"
// SyncLogger synclogger;
// int main(){
//     LOG("程序启动");
//     int count = 1;

    
//     auto start = std::chrono::high_resolution_clock::now();

//     while(count <= 1000000){
//         synclogger.log("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐",__FILE__,__LINE__,level::DEBUG);
//     }


//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
//     return 0;
// }


// int main(){
//     LOG("程序启动");
//     int count = 1;

//     LogStarter::instance();
//     auto start = std::chrono::high_resolution_clock::now();

//     while(count <= 1000000){
//         debug_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         info_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         warn_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         error_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         fatal_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         debug("模拟日志，日志序号%d🤓🌹⭐",count++);
//         info("模拟日志，日志序号%d🤓🌹⭐",count++);
//         warn("模拟日志，日志序号%d🤓🌹⭐",count++);
//         error("模拟日志，日志序号%d🤓🌹⭐",count++);
//         fatal("模拟日志，日志序号%d🤓🌹⭐",count++);
//     }


//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
//     return 0;
// }


///////////////////////////////////////

//多线程 异步
// void threadFunc(){
//     for(int i=0;i<333333;i++){
//         debug_str("模拟日志，日志序号"+std::to_string(i)+"🤓🌹⭐");
//     }

//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     std::cout << "线程" << std::this_thread::get_id() << " 用时: " << duration << std::endl;
// }
// int main(){
//     LOG("程序启动");
//     LogStarter::instance();

//     start = std::chrono::high_resolution_clock::now();
//     ////////////////
    
//     int n = 3;
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


//多线程同步（用同一个日志器写）
#include <thread>
#include <vector>
SyncLogger synclogger;

void threadFunc(){
    for(int i=0;i<333333;i++){
        synclogger.log("模拟日志，日志序号"+std::to_string(i)+"🤓🌹⭐",__FILE__,__LINE__,level::DEBUG);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "线程" << std::this_thread::get_id() << " 用时: " << duration << std::endl;
}
int main(){
    LOG("程序启动");
    start = std::chrono::high_resolution_clock::now();
    ////////////////
    
    int n = 3;
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