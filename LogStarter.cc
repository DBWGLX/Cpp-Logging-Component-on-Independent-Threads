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
    printf("[threadFunction_producer] Thread_producer is starting!\n");

    std::shared_ptr<char[]> buffer = double_log_buffer.getBuffer1();
    if (buffer == nullptr) {
        fprintf(stderr, "Error: buffer is null after swap!\n");
        exit(1);
    }
    int cur_pos = 0;

    printf("[threadFunction_producer] 生产者开始工作\n");

    while(logThreadisRunning){
        std::unique_lock<std::mutex> lock(logMutex1);
        cv1.wait(lock,[&]{return task_queue.size() > 0 || !logThreadisRunning;});

        printf("[threadFunction_producer] Thread_producer is dealing task!\n");

        //处理任务队列
        while(task_queue.size()){
            printf("[threadFunction_producer] 当前task_queue.size()：%d\n", task_queue.size());
            LogInfo aim = task_queue.pop();
            printf("【DEBUG】正确获取任务队列任务\n");
            std::string log_msg_str = aim.logFormat();
            printf("[threadFunction_producer] 要写的内容：%s",log_msg_str.c_str());
            
            //x 放不下时交换
            if(cur_pos+log_msg_str.size()+1 >= double_log_buffer.size()){

                printf("[threadFunction_producer] 生产者放不下了，即将交换缓冲区\n");

                while(!double_log_buffer.isBuffer2Clear()){
                    printf("[threadFunction_producer] double_log_buffer 还未清空\n");
                    sleep(1);
                }
                double_log_buffer.swap();
                notify_consumer();
                buffer = double_log_buffer.getBuffer1();
                cur_pos = 0;
                if (buffer == nullptr) {
                    fprintf(stderr, "Error: buffer is null after swap!\n");
                    exit(1);
                }
            }

            
            memcpy(buffer.get()+cur_pos,log_msg_str.c_str(),log_msg_str.size()+1);
            cur_pos += log_msg_str.size();

            //x 90%时也交换
            if(cur_pos >= double_log_buffer.size()*0.9){
                
                printf("[threadFunction_producer] 生产者缓冲区已达90%，即将交换缓冲区\n");
                
                //交换
                while(!double_log_buffer.isBuffer2Clear()){
                    printf("[threadFunction_producer] double_log_buffer 还未清空\n");
                    sleep(1);
                }
                double_log_buffer.swap();
                printf("[threadFunction_producer] 交换完成\n");
                notify_consumer();
                buffer = double_log_buffer.getBuffer1();
                cur_pos = 0;
                if (buffer.get() == nullptr) {
                    fprintf(stderr, "Error: buffer is null after swap!\n");
                    exit(1);
                }
            }
            
            printf("[threadFunction_producer] 当前位置cur_pos：%d\n", cur_pos);
        }
        printf("[threadFunction_producer] 当前任务队列已处理完毕\n");
    }
}

void dbwg::LogStarter::threadFunction_consumer(){
    printf("[threadFunction_consumer] Thread_consumer is starting!\n");
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

    printf("[threadFunction_consumer] 消费者开始工作\n");

    //开始工作
    while(logThreadisRunning){
        if(double_log_buffer.isBuffer2Clear()){
            printf("[threadFunction_consumer] 消费者等待\n");
            std::unique_lock<std::mutex> lock(logMutex2);
            cv2.wait(lock,[&]{return !double_log_buffer.isBuffer2Clear() || !logThreadisRunning;});//存在【虚假唤醒】：以确保线程不会因为等待条件变量而无限期地阻塞，错过一些重要的系统事件。
        }

        printf("[threadFunction_consumer] Thread_consumer is writing!\n");

        int rs = fprintf(file.get(), "%s",double_log_buffer.getBuffer2().get());
        //printf("[threadFunction_consumer] writing finishes : %s\n");

        if(rs<0)printf("[threadFunction_consumer] Error writing to file.\n");
        fflush(file.get());

        double_log_buffer.clearBuffer2();

        //roll-file
        cur_size += rs;
        if(cur_size >= _log_file_size){

            printf("[threadFunction_consumer] 文件写满，滚动文件\n");

            file.reset(files_roller.roll_file());
            if(file == NULL){
                perror("[threadFunction_consumer]  Error opening file");
                return;
            }
            cur_size = 0;
        }
        printf("[threadFunction_consumer] 单次落地完毕\n");
    }
}


//初始化
dbwg::LogStarter::LogStarter(int buf_size,int roll_size,int log_file_size)
    : double_log_buffer(buf_size),files_roller(roll_size),_log_file_size(log_file_size) {
    if(!LogStarter::logThreadisRunning){
        //启动线程
        LogStarter::logThreadisRunning = true;
        std::thread logThread1(&LogStarter::threadFunction_producer,this);
        if(!logThread1.joinable()){
            std::cerr << "[LogStarter] Failed to create logThread1." << std::endl;
            LogStarter::logThreadisRunning = false;
            return;
        }
        std::thread logThread2(&LogStarter::threadFunction_consumer,this);
        if (!logThread2.joinable()) {
            std::cerr << "[LogStarter] Failed to create logThread2." << std::endl;
            LogStarter::logThreadisRunning = false;
            logThread1.join(); // 确保之前创建的线程正常结束
            return;
        }


        logThread1.detach();
        logThread2.detach();
        printf("[LogStarter] LogStarter starts\n");
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
            //printf("[log] task_queue.size() == %d > 10，等待生产者组织。\n",task_queue.size());
        }
        std::lock_guard<std::mutex>lock(logMutex0);
        task_queue.push(LogInfo(message,levl,file,line));
        notify_producer();

        printf("[log] 调用了log，当前队列大小：%d\n", task_queue.size());
    }
    else{
        printf("[LogStarter::log] \"dbwg::LogStarter\" is not running now!\n");
        exit(1);
    }
}
