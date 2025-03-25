#include "dbwg_logger.hpp"
void dbwg::log1(dbwg::level::level levl, const char* file, int line , const char* fmt, ...){
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

void dbwg::log1(dbwg::level::level levl, const char* file, int line , std::string str){
    dbwg::LogStarter::instance().log(str,file,line,levl);
}

void dbwg::flush(){
    dbwg::LogStarter::instance().flush();
}