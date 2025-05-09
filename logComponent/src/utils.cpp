#include "utils.hpp"

namespace dbwg{

    //获取当前时间的字符串 "2025-01-17 12:38:02"
    std::string utils::now(){
        std::time_t now = std::time(nullptr);
        std::tm* local_tm = std::localtime(&now);
        std::stringstream str;
        str << (local_tm->tm_year+1900) << "-"
            << std::setw(2) << std::setfill('0') << (local_tm->tm_mon + 1) << "-"
            << std::setw(2) << std::setfill('0') << local_tm->tm_mday << " "
            << std::setw(2) << std::setfill('0') << local_tm->tm_hour << ":"
            << std::setw(2) << std::setfill('0') << local_tm->tm_min << ":"
            << std::setw(2) << std::setfill('0') << local_tm->tm_sec;
        return str.str();
    }
    //创建目录 "./dir"
    int utils::mkdir(const std::string pathname,mode_t mode){
        if(::mkdir(pathname.c_str(),mode)==0)
            return 0;
        else{
            if (errno == EEXIST)//已存在
                return 0;
            perror("mkdir failed");      
        }
        return 1;
    }


}