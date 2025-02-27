#pragma once
#ifndef DBWGLX_LOGINFO
#define DBWGLX_LOGINFO
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
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
        std::string _time;
        level::level _levl;
        pid_t _pid;
        std::string _filename;
        int _line;
        std::string _message;
    };
}



