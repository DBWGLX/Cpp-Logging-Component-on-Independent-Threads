#include "LogStarter.hpp"

struct FileCloser {
    void operator()(FILE* file) const {
        if (file) {
            std::fclose(file);
        }
    }
};

// LogStarter 静态成员变量初始化
bool dbwg::LogStarter::logThreadisRunning = false;

//线程函数
void dbwg::LogStarter::threadFunction_producer(){
    printf("Thread_producer is starting!\n");

    std::shared_ptr<char[]> buffer = double_log_buffer.getBuffer1();
    if (buffer == nullptr) {
        fprintf(stderr, "Error: buffer is null after swap!\n");
        exit(1);
    }
    int cur_pos = 0;

    printf("生产者开始工作\n");

    while(logThreadisRunning){
        std::unique_lock<std::mutex> lock(logMutex1);
        cv1.wait(lock,[&]{return task_queue.size() > 0;});

        printf("Thread_producer is dealing task!\n");

        //处理任务队列
        while(task_queue.size()){
            LogInfo aim = task_queue.pop();
            std::string log_msg_str = aim.logFormat();
            printf("要写的内容：%s   当前位置：%d\n",log_msg_str.c_str(),cur_pos);
            //放不下时交换
            if(cur_pos+log_msg_str.size()+1 >= double_log_buffer.size()){

                printf("生产者放不下了，即将交换缓冲区\n");

                while(double_log_buffer.getBuffer1()[0] != '\0'){
                    sleep(0.5);
                }
                double_log_buffer.swap();
                notify_consumer();
                buffer = double_log_buffer.getBuffer1();
                if (buffer == nullptr) {
                    fprintf(stderr, "Error: buffer is null after swap!\n");
                    exit(1);
                }
            }

            strcpy(buffer.get()+cur_pos,log_msg_str.c_str());
            cur_pos += log_msg_str.size();
            //90%时也交换
            if(cur_pos >= double_log_buffer.size()*0.9){
                
                printf("2 生产者放不下了，即将交换缓冲区\n");
                
                //交换
                while(double_log_buffer.getBuffer2()[0] != '\0'){
                    sleep(0.5);
                }
                double_log_buffer.swap();
                notify_consumer();
                buffer = double_log_buffer.getBuffer1();
                if (buffer == nullptr) {
                    fprintf(stderr, "Error: buffer is null after swap!\n");
                    exit(1);
                }
            }
        }
    }
}

void dbwg::LogStarter::threadFunction_consumer(){
    printf("Thread_consumer is starting!\n");
    //创建log目录
    if (utils::mkdir("log", 0777) != 0) {//我的mkdir，即使存在也返回0
        perror("Error creating log directory");
        return;
    }
    //先打开一个文件
    std::unique_ptr<FILE,FileCloser> file(files_roller.roll_file());//每次启动,日志文件都会重新开始
    if(file == NULL){
        perror("Error opening file");
        return;
    }
    int cur_size = 0;//当前文件大小，超了就要roll

    printf("消费者开始工作\n");

    //开始工作
    while(logThreadisRunning){
        //条件变量
        std::unique_lock<std::mutex> lock(logMutex2);
        cv2.wait(lock);//存在【虚假唤醒】：以确保线程不会因为等待条件变量而无限期地阻塞，错过一些重要的系统事件。

        printf("Thread_consumer is writing!\n");

        int rs = fprintf(file.get(), "%s",double_log_buffer.getBuffer2());
        if(rs<0)printf("Error writing to file.\n");
        fflush(file.get());

        double_log_buffer.clearBuffer2();
        double_log_buffer.swap();

        //roll-file
        cur_size += rs;
        if(cur_size >= _log_file_size){

            printf("文件写满，滚动文件\n");

            file.reset(files_roller.roll_file());
            if(file == NULL){
                perror("[LogStarter::threadFunction] Error opening file");
                return;
            }
            cur_size = 0;
        }
    }
}


//初始化
dbwg::LogStarter::LogStarter(int buf_size,int roll_size,int log_file_size)
    : double_log_buffer(buf_size),files_roller(roll_size),_log_file_size(log_file_size) {
    if(!LogStarter::logThreadisRunning){
        //启动线程
        LogStarter::logThreadisRunning = true;
        std::thread logThread1(&LogStarter::threadFunction_producer,this);
        std::thread logThread2(&LogStarter::threadFunction_consumer,this);
        logThread1.detach();
        logThread2.detach();
        //单例
        printf("LogStarter\n");
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
    cv1.notify_one();
}
void dbwg::LogStarter::notify_consumer(){
    cv2.notify_one();
}


//独立线程日志组件。
//启动器启动后，log("")即可。
void dbwg::LogStarter::log(std::string message,const char* file, int line,level::level levl){
    if(isRunning()){
        while(task_queue.size()>10){
            sleep(1);
        }
        std::lock_guard<std::mutex>lock(logMutex0);
        task_queue.push(LogInfo(message,levl,file,line));
        notify_producer();

        printf("调用了log，当前队列大小：%d\n", task_queue.size());
    }
    else{
        printf("[LogStarter::log] Please execute \"dbwg::LogStarter::start\" ");
        exit(1);
    }
}
