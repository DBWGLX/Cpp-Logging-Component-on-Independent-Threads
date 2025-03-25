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

namespace dbwg{
    void log1(dbwg::level::level levl, const char* file, int line , const char* fmt, ...);
    void log1(dbwg::level::level levl, const char* file, int line , std::string str);
    void flush();
}


#define debug(fmt, ...) dbwg::log1(dbwg::level::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define info(fmt, ...) dbwg::log1(dbwg::level::INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define warn(fmt, ...) dbwg::log1(dbwg::level::WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define error(fmt, ...) dbwg::log1(dbwg::level::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__);
#define fatal(fmt, ...) dbwg::log1(dbwg::level::FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__);

#define debug_str(fmt) dbwg::log1(dbwg::level::DEBUG, __FILE__, __LINE__, fmt);
#define info_str(fmt) dbwg::log1(dbwg::level::INFO, __FILE__, __LINE__, fmt);
#define warn_str(fmt) dbwg::log1(dbwg::level::WARN, __FILE__, __LINE__, fmt);
#define error_str(fmt) dbwg::log1(dbwg::level::ERROR, __FILE__, __LINE__, fmt);
#define fatal_str(fmt) dbwg::log1(dbwg::level::FATAL, __FILE__, __LINE__, fmt);

#define logger_flush() dbwg::flush();