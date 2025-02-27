#include "LogStarter.hpp"

// 自定义删除器，用于智能指针关闭 FILE*
struct FileCloser {
    void operator()(FILE* file) const {
        if (file) {
            std::fclose(file);
        }
    }
};

// LogStarter 静态成员变量初始化
bool dbwg::LogStarter::logThreadisRunning = false;

//初始化
void dbwg::LogStarter(int buf_size,int roll_size,int log_file_size)
    : logbuff(buf_size),rf(roll_size),_log_file_size(log_file_size) {
    if(!logThreadisRunning){
        //启动线程
        std::thread logThread1(threadFunction_producer);
        std::thread logThread2(threadFunction_consumer);
        logThread1.detach();
        logThread2.detach();
        //单例
        logThreadisRunning = true;
        printf("[LogStarter::start] logwriter线程已启动\n");
    }
}

bool dbwg::LogStarter::isRunning(){
    return logThreadisRunning;
} 

//唤醒线程
void dbwg::LogStarter::notify_producer(){
    cv1.notify_one();
}
void dbwg::LogStarter::notify_consumer(){
    cv2.notify_one();
}

void dbwg::LogStarter::threadFunction_producer(){
    std::shared_ptr<char[]> buffer = double_log_buffer.getBuffer1();
    int cur_pos = 0;
    while(logThreadisRunning){
        std::unique_lock<std::mutex> lock(logMutex1);
        cv1.wait(lock,[&](return task_queue.size() > 0));
        while(task_queue.size()){
            LogInfo aim = task_queue.pop();
            string log_msg_str = aim.logFormat();
            if(cur_pos+log_msg_str.size() >= double_log_buffer.size()){
                //交换
                while()
            }

            strcpy(buffer,log_msg_str.c_str());
            cur_pos += log_msg_str.size();
        }
    }
}

void dbwg::LogStarter::threadFunction_consumer(){
    //创建log目录
    if (utils::mkdir("log", 0777) != 0) {//我的mkdir，即使存在也返回0
        perror("Error creating log directory");
        return;
    }
    //先打开一个文件
    unique_ptr<FILE,FileCloser> file = roll_file();//每次启动,日志文件都会重新开始
    if(file == NULL){
        perror("Error opening file");
        return;
    }
    int cur_size = 0;//当前文件大小，超了就要roll
    //开始工作
    while(logThreadisRunning){
        //条件变量
        std::unique_lock<std::mutex> lock(logMutex2);
        cv2.wait(lock);//【虚假唤醒】以确保线程不会因为等待条件变量而无限期地阻塞，错过一些重要的系统事件。

        int rs = fprintf(file, "%s",logbuff.getBuffer2());
        if(rs<0)printf("Error writing to file.\n");
        fflush(file);

        logbuff.clearBuffer2();
        logbuff.swap();

        //roll-file
        cur_size += rs;
        if(cur_size >= _log_file_size){
            fclose(file);
            file = roll_file();
            if(file == NULL){
                perror("[LogStarter::threadFunction] Error opening file");
                return;
            }
            cur_size = 0;
        }
    }
}

//独立线程日志组件。
//启动器启动后，log("")即可。
void dbwg::LogStarter::log(std::string message,const char* file, int line,level::level levl){
    if(isRunning()){
        std::lock_guard<std::mutex>lock(logMutex0);
        task_queue.push(LogInfo(message,levl,file,utils::now()));
        if(task_queue.size()>10){
            notify_producer();
        }
    }
    else{
        printf("[LogStarter::log] Please execute \"dbwg::LogStarter::start\" ");
        exit(1);
    }
}
