#pragma once
#ifndef DBWGLX_UTILS
#define DBWGLX_UTILS
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>//manipulate
#include <sys/stat.h>
#include <stdio.h>


namespace dbwg{
    class utils{
        public:
            //获取当前时间的字符串 "2025-01-17 12:38:02"
            static std::string now();
            //创建目录 "./dir"
            static int mkdir(const std::string pathname,mode_t mode = 0755);
    };

}

#endif