#ifndef DBWGLX_LOG_SINK
#define DBWGLX_LOG_SINK
#include <iostream>
#include <condition_variable>
#include <string>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <memory>
#include "Level.hpp"
#include "LogInfo.hpp"
#include "DoubleBuffer.hpp"
#include "FilesRoller.hpp"
#include "LogProducer.hpp"

namespace dbwg{
    class LogStarter{
    public:
        //构造时会创建线程方法
        LogStarter(int buf_size = 1024/*缓冲区大小*/,
            int roll_size = 10/*滚动文件数目*/,int log_file_size = 10000/*日志文件大小*/);
        
        static bool isRunning();
        static LogStarter& instance();
        
        //启动器启动后，调用方法log("")即可写日志。
        //此时会把日志先放入一个队列中
        void log(std::string message,const char* file = __FILE__, int line = __LINE__,level::level levl = level::DEBUG);
    private:
        //多个线程同时写日志，先把结构体传给任务队列
        //接下来由生产组织线程组织成字符串放入双缓冲区，满时交换缓冲区，唤醒消费者
        void threadFunction_producer();
        //消费者线程讲缓冲区内日志写入滚动文件
        void threadFunction_consumer();
        void notify_producer();//唤醒等待进程
        void notify_consumer();

        //任务队列锁
        std::mutex logMutex0;
        //生产组织者
        std::mutex logMutex1;
        std::condition_variable cv1;//条件变量，日志阻塞写
        //消费者线程的条件变量
        std::mutex logMutex2;
        std::condition_variable cv2;//条件变量，日志阻塞写
        //任务队列
        LogProducer<LogInfo>task_queue;
        //双缓冲区
        DoubleBuffer double_log_buffer;
        //日志滚动文件
        FilesRoller files_roller;
        
        int _log_file_size;
        static bool logThreadisRunning;//日志器状态
    };
}


#endif