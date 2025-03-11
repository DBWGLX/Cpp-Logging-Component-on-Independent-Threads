#ifndef SYNCLOGGER
#define SYNCLOGGER
#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <mutex>
#include <cstring>
#include <sys/mman.h>
#include "FilesRoller.hpp"
#include "UniqueFD.hpp"
#include "utils.hpp"
#include "LogInfo.hpp"
#include "Level.hpp"

#define BUFF_SIZE 100000  /*缓冲区大小*/
#define ROLL_FIlES 1024  /*滚动文件数目*/
#define LOG_FILE_SIZE 10000000  /*日志文件大小*/

namespace dbwg{
    class SyncLogger{
    public:
        SyncLogger();
        void rollFile();
        void log(std::string message,const char* file, int line,level::level levl);
    private:
        FilesRoller files_roller;
        int cur_size;

        std::mutex mutex;
        UniqueFD fileFd;
        char* mapped;
    };
}


#endif