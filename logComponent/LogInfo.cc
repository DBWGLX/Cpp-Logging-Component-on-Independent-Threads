#include "LogInfo.hpp"

// 2025-01-17 12:38:02 [WARN][427908][main.cc:15] 开始循环
dbwg::LogInfo::LogInfo(std::string message,dbwg::level::level levl,const char* file, int line)
    :_time(dbwg::utils::now()),_levl(levl),_pid(getpid()),thisId(std::this_thread::get_id()),_filename(file),_line(line),_message(message)
{}

//组织为字符串
std::string dbwg::LogInfo::logFormat(){
    std::stringstream ss;
    ss << _time << " ";
    ss << "[" << level::level2str(_levl) << "]";
    ss << "[" << _pid << "]";
    ss << "[" << thisId << "]";
    ss << "[" << _filename << ":" << _line << "] ";
    ss << _message;
    ss << "\n";
    return ss.str();
}



