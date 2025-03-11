#include "SyncLogger.hpp"

namespace dbwg{

    SyncLogger::SyncLogger(){
        if (utils::mkdir("log", 0777) != 0) {//我的mkdir，即使存在也返回0
            perror("Error creating log directory");
        }
        //init
        rollFile();
    }
    void SyncLogger::rollFile(){
        fileFd = files_roller.roll_file();
        if (fileFd.get() == -1) {
            perror("Error opening log file");
            return;
        }
        cur_size = 0;
    }
    void SyncLogger::log(std::string message,const char* file, int line,level::level levl){
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


}

    // mmap版本的实现
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