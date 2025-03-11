#pragma once
#include "Level.hpp"//日志等级
#include "LogInfo.hpp"//日志体
#include "LogStarter.hpp"//异步日志写
#include "utils.hpp"//工具函数
#include "SyncLogger.hpp"//同步写
#include <cstdarg>
#include <stdio.h>
#include <string>


#define LOG(fmt, ...) printf("%s [%s:%d] " fmt "\n",dbwg::utils::now().c_str(),__FILE__,__LINE__,##__VA_ARGS__)

void log1(dbwg::level::level levl, const char* file, int line , const char* fmt, ...){
    constexpr size_t buffer_size = 512;
    char buffer[buffer_size];

    va_list args;
    va_start(args, fmt);
    int size = vsnprintf(buffer,buffer_size,fmt,args);
    va_end(args);

    if(size < 0 || size >= buffer_size){
        perror("[log1] 格式化失败或内容被截断\n");
        return;
    }

    dbwg::LogStarter::instance().log(std::string(buffer),file,line,levl);
}

void log1(dbwg::level::level levl, const char* file, int line , std::string str){
    dbwg::LogStarter::instance().log(str,file,line,levl);
}


#define debug(fmt, ...) log1(dbwg::level::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define info(fmt, ...) log1(dbwg::level::INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define warn(fmt, ...) log1(dbwg::level::WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define error(fmt, ...) log1(dbwg::level::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define fatal(fmt, ...) log1(dbwg::level::FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__);

#define debug_str(fmt) log1(dbwg::level::DEBUG, __FILE__, __LINE__, fmt);
#define info_str(fmt) log1(dbwg::level::INFO, __FILE__, __LINE__, fmt);
#define warn_str(fmt) log1(dbwg::level::WARN, __FILE__, __LINE__, fmt);
#define error_str(fmt) log1(dbwg::level::ERROR, __FILE__, __LINE__, fmt);
#define fatal_str(fmt) log1(dbwg::level::FATAL, __FILE__, __LINE__, fmt);