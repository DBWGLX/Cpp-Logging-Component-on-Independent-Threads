#pragma once
#ifndef DBWGLX_LEVEL
#define DBWGLX_LEVEL
#include<iostream>
#endif
namespace dbwg{
    namespace level{
        // 2025-01-17 12:38:02 [WARN][427908][main.cc:15] 开始循环
        enum level{
            OFF = 0,
            FATAL,
            ERROR,
            WARN,
            INFO,
            DEBUG,
        };
        std::string level2str(level levl);
    }
}