#ifndef DBWGLX_LOG_SINK
#define DBWGLX_LOG_SINK
#include <iostream>
#include <condition_variable>
#include <string>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <memory>
#include <fcntl.h>
#include <sys/mman.h>
#include "Level.hpp"
#include "LogInfo.hpp"
#include "DoubleBuffer.hpp"
#include "FilesRoller.hpp"
#include "UniqueFD.hpp"

#define BUFF_SIZE 100000  /*缓冲区大小*/
#define ROLL_FIlES 1024  /*滚动文件数目*/
#define LOG_FILE_SIZE 10000000  /*日志文件大小*/

//异步线程写日志
namespace dbwg{
    class LogStarter{
    public:
        //构造时会创建线程方法
        LogStarter(int buf_size = BUFF_SIZE/*缓冲区大小*/,
            int roll_size = ROLL_FIlES/*滚动文件数目*/,int log_file_size = LOG_FILE_SIZE/*日志文件大小*/);
        ~LogStarter();

        static bool isRunning();
        static LogStarter& instance();
        
        //启动器启动后，调用方法log("")即可写日志。
        //此时会把日志先放入一个队列中
        void log(std::string message,const char* file = __FILE__, int line = __LINE__,level::level levl = level::DEBUG);
    private:
        //消费者线程讲缓冲区内日志写入滚动文件
        void threadFunction_consumer();
        void notify_producer();//唤醒等待进程
        void notify_consumer();

        //生产者锁
        std::mutex logMutex1;
        std::condition_variable cv1;//条件变量，日志阻塞写
        //消费者线程
        std::thread logThread;
        std::mutex logMutex2;
        std::condition_variable cv2;//条件变量，日志阻塞写
        //双缓冲区
        DoubleBuffer double_log_buffer;
        //日志滚动文件
        FilesRoller files_roller;
        
        int _log_file_size;
        static bool logThreadisRunning;//日志器状态
    };
}


#endif