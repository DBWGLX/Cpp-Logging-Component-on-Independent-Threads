#pragma once
#ifndef DBWGLX_LOG_SINK
#define DBWGLX_LOG_SINK
#include <iostream>
#include <condition_variable>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <cassert>
#include "ring.hpp"
#include "level.hpp"
#include "logInfo.hpp"
#endif

namespace dbwg{
    class LogStarter{
    public:
        static Ring<LogInfo> *getRing();
        //启动
        static void start();
        static void threadFunction();
        static bool isRunning();
        static void notify_one();
    private:
        static FILE* roll_file();

        static Ring<LogInfo> *ring;
        static std::mutex container_mutex;//[单例懒汉]线程安全
        static std::mutex logMutex;
        static bool logThreadisRunning;
        static std::condition_variable cv;

        static std::vector<std::string>rfns;//roll-file-names
        static int rp;
    };

    //独立线程日志组件。
    //启动器启动后，log("")即可。
    void log(std::string message,const char* file = __FILE__, int line = __LINE__,level::level levl = level::DEBUG);
}