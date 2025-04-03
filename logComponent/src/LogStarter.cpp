#include "LogStarter.hpp"

// LogStarter 静态成员变量初始化
bool dbwg::LogStarter::logThreadisRunning = false;

void dbwg::LogStarter::threadFunction_consumer(){
    //创建log目录
    if (utils::mkdir("log", 0777) != 0) {//我的mkdir，即使存在也返回0
        perror("Error creating log directory");
        return;
    }
    //先打开一个文件
    UniqueFD fileFd(files_roller.roll_file());//每次启动,日志文件都会重新开始
    if (fileFd.get() == -1) {
        perror("Error opening log file");
        return;
    }

    int cur_size = 0;//当前日志大小，超了就要roll
    //# 开始工作
    while(logThreadisRunning){
        std::unique_lock<std::mutex> lock(logMutex2);
        cv2.wait(lock,[&]{return double_log_buffer.isBuffer1Full() || !logThreadisRunning;});

        int len = double_log_buffer.get_index();
        double_log_buffer.swap();
        notify_producer();

        //足够大时滚动文件
        if(cur_size+len >= _log_file_size){
            fileFd.reset(files_roller.roll_file());
            if (fileFd.get() == -1) {
                perror("Error opening log file");
                return;
            }
            cur_size = 0;
        }

        ssize_t written = write(fileFd.get(),double_log_buffer.getBuffer2().get(),len);
        if(written == -1){
            perror("write failed");
            return;
        }
        if(fsync(fileFd.get()) == -1){
            perror("fsync failed");
            return;
        }

        cur_size += written;
    }
}

//初始化
dbwg::LogStarter::LogStarter(int buf_size,int roll_size,int log_file_size)
    : double_log_buffer(buf_size),files_roller(roll_size),_log_file_size(log_file_size) {
    if(!LogStarter::logThreadisRunning){
        //启动线程
        LogStarter::logThreadisRunning = true;

        logThread = std::thread(&LogStarter::threadFunction_consumer,this);
        if (!logThread.joinable()) {
            LogStarter::logThreadisRunning = false;
            logThread.join(); // 确保之前创建的线程正常结束
            return;
        }

        printf("[LogStarter] LogStarter starts\n");
    }
}
dbwg::LogStarter::~LogStarter(){
    logThreadisRunning = false;
    notify_consumer();
    if(logThread.joinable()){
        logThread.join();
    }
}

bool dbwg::LogStarter::isRunning(){
    return logThreadisRunning;
} 

dbwg::LogStarter& dbwg::LogStarter::instance() {
    static LogStarter instance;
    return instance;
}

//唤醒线程
void dbwg::LogStarter::notify_producer(){
    cv1.notify_all();
}
void dbwg::LogStarter::notify_consumer(){
    double_log_buffer.setBuffer1Full();
    cv2.notify_one();
}


void dbwg::LogStarter::log(std::string message,const char* file, int line,level::level levl){
    if(isRunning()){
        std::string log_msg_str = LogInfo(message,levl,file,line).logFormat();
        std::unique_lock<std::mutex>lock(logMutex1);
        while(double_log_buffer.get_index() + log_msg_str.size() + 1 >= double_log_buffer.size() || double_log_buffer.isBuffer1Full()){   //写不下，或者是满状态      
            if(log_msg_str.size() >= double_log_buffer.size()){
                perror("[logger] 日志过大，请增大日志组件缓冲区或减少日志内容");
                std::cerr << "大日志：" << message.substr(0,50) << "..." << std::endl;
            }
            notify_consumer();//里面设置了满flag
            cv1.wait(lock,[&]{return !double_log_buffer.isBuffer1Full();});
        }

        // 写入 写缓冲区
        int cur_index = double_log_buffer.get_index();
        std::shared_ptr<char[]> buffer = double_log_buffer.getBuffer1();
        memcpy(buffer.get()+cur_index, log_msg_str.c_str(),log_msg_str.size()+1);
        double_log_buffer.addIndex(log_msg_str.size());
    }
    else{
        perror("[LogStarter::log] \"dbwg::LogStarter\" is not running now!\n");
        exit(1);
    }
}

void dbwg::LogStarter::flush() {
    std::unique_lock<std::mutex> lock(logMutex1);
    notify_consumer();
}


// mmap策略
// void dbwg::LogStarter::threadFunction_consumer(){
//     //创建log目录
//     if (utils::mkdir("log", 0777) != 0) {//我的mkdir，即使存在也返回0
//         perror("Error creating log directory");
//         return;
//     }
//     //先打开一个文件
//     UniqueFD fileFd(files_roller.roll_file());//每次启动,日志文件都会重新开始
//     if (fileFd.get() == -1) {
//         perror("Error opening log file");
//         return;
//     }
//     if(ftruncate(fileFd.get(),_log_file_size) == -1){
//         perror("ftruncate failed");
//         return;
//     }

//     char* mapped = (char *)mmap(NULL, LOG_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileFd.get(), 0);
//     if(mapped == MAP_FAILED){
//         perror("mmap failed");
//         return;
//     }

//     int cur_size = 0;//当前日志大小，超了就要roll
//     //# 开始工作
//     while(logThreadisRunning){
//         std::unique_lock<std::mutex> lock(logMutex2);
//         cv2.wait(lock,[&]{return double_log_buffer.isBuffer1Full() || !logThreadisRunning;});

//         int len = double_log_buffer.get_index();
//         double_log_buffer.swap();
//         notify_producer();

//         //足够大时滚动文件
//         if(cur_size+len >= _log_file_size){
//             munmap(mapped, LOG_FILE_SIZE);
//             fileFd.reset(files_roller.roll_file());
//             if(ftruncate(fileFd.get(),_log_file_size) == -1){
//                 perror("ftruncate failed");
//                 return;
//             }
//             mapped = (char *)mmap(NULL, LOG_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileFd.get(), 0);
//             if(mapped == MAP_FAILED){
//                 perror("mmap failed");
//                 return;
//             }
//             cur_size = 0;
//         }

//         memcpy(mapped + cur_size, double_log_buffer.getBuffer2().get(), len);
//         msync(mapped + cur_size, len, MS_SYNC);
//         cur_size += len;
//     }
// }
