#pragma once
#ifndef DBWGLX_UTILS
#define DBWGLX_UTILS
#include <iostream>
#include <ctime>
#include <sstream>
#include <sys/stat.h>
#include <stdio.h>
#endif

namespace dbwg{
    class utils{
        public:
            //获取当前时间的字符串 "2025-01-17 12:38:02"
            static std::string now(){
                std::time_t now = std::time(nullptr);
                std::tm* local_tm = std::localtime(&now);
                std::stringstream str;
                str << (local_tm->tm_year+1900) << "-"
                    << (local_tm->tm_mon + 1) << "-"
                    << local_tm->tm_mday << " "
                    << local_tm->tm_hour << ":"
                    << local_tm->tm_min << ":"
                    << local_tm->tm_sec;
                return str.str();
            }
            //创建目录 "./dir"
            static int mkdir(const std::string pathname,mode_t mode = 0755){
                if(::mkdir(pathname.c_str(),mode)==0)
                    return 0;
                else{
                    if (errno == EEXIST)
                        return 0;
                    perror("mkdir failed");      
                }
                return 1;
            }
    };

}