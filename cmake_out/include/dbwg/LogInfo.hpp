#pragma once
#ifndef DBWGLX_LOGINFO
#define DBWGLX_LOGINFO
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include "Level.hpp"
#include "utils.hpp"
#endif

namespace dbwg{
    class LogInfo{
    public:
        // 2025-01-17 12:38:02 [WARN][427908][main.cc:15] 开始循环
        LogInfo(std::string message = "",level::level levl = level::DEBUG,
            const char* file = __FILE__, int line = __LINE__);
        //组织为字符串
        std::string logFormat();
    private:
        std::string _time;//时间
        level::level _levl;//日志等级
        pid_t _pid;//进程id
        std::thread::id thisId;//线程id
        std::string _filename;//文件名
        int _line;//行号
        std::string _message;//内容
    };
}



