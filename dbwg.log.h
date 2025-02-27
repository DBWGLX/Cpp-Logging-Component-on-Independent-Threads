#pragma once
#include "Level.hpp"//日志等级
#include "LogInfo.hpp"//日志体
#include "LogSink.hpp"//日志写
#include "utils.hpp"//工具函数
#include <cstdarg>


#define LOG(fmt, ...) printf("%s [%s:%d] " fmt "\n",dbwg::utils::now().c_str(),__FILE__,__LINE__,##__VA_ARGS__)
#define LOG1(message) dbwg::log(message,__FILE__,__LINE__)
#define LOG11(message, level) dbwg::log(message,__FILE__,__LINE__,level)