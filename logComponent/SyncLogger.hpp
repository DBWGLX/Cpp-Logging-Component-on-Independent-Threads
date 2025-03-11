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
        SyncLogger(){
            if (utils::mkdir("log", 0777) != 0) {//我的mkdir，即使存在也返回0
                perror("Error creating log directory");
            }
            //init
            rollFile();
        }
        // mmap的实现
        // void rollFile(){
        //     fileFd = files_roller.roll_file();
        //     if (fileFd.get() == -1) {
        //         perror("Error opening log file");
        //         return;
        //     }
        //     if(ftruncate(fileFd.get(),LOG_FILE_SIZE) == -1){
        //         perror("ftruncate failed");
        //         return;
        //     }
        //     mapped = (char*)mmap(NULL,LOG_FILE_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fileFd.get(),0);
        //     if(mapped == MAP_FAILED){
        //         perror("mmap failed");
        //         return;
        //     }
        //     cur_size = 0;
        // }
        void rollFile(){
            fileFd = files_roller.roll_file();
            if (fileFd.get() == -1) {
                perror("Error opening log file");
                return;
            }
            cur_size = 0;
        }
        void log(std::string message,const char* file, int line,level::level levl){
            std::string log_msg_str = LogInfo(message,levl,file,line).logFormat();
            int len = log_msg_str.size();
            std::unique_lock<std::mutex>lock(mutex);
            if(cur_size + len > LOG_FILE_SIZE){
                rollFile();
            }

            ssize_t written = write(fileFd.get(),log_msg_str.c_str(),len);
            if(written == -1){
                perror("write failed");
                return;
            }
            // if(fsync(fileFd.get()) == -1){
            //     perror("fsync failed");
            //     return;
            // }
            cur_size += written;
        }
    private:
        FilesRoller files_roller;
        int cur_size;

        std::mutex mutex;
        UniqueFD fileFd;
        char* mapped;
    };

}


#endif