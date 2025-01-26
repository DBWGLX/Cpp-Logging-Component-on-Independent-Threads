#include "logSink.hpp"

#define MAX_LOG_FILE_SIZE 10000//10000000 //10MB

// LogStarter 静态成员变量初始化
dbwg::Ring<dbwg::LogInfo>* dbwg::LogStarter::ring = nullptr;
std::mutex dbwg::LogStarter::container_mutex;
std::mutex dbwg::LogStarter::logMutex;
bool dbwg::LogStarter::logThreadisRunning = false;
std::condition_variable dbwg::LogStarter::cv;
std::vector<std::string> dbwg::LogStarter::rfns = std::vector<std::string>(10);//roll-file-names
int dbwg::LogStarter::rp = 0;

dbwg::Ring<dbwg::LogInfo>* dbwg::LogStarter::getRing(){
    if(ring == nullptr){
        std::lock_guard<std::mutex> lock(container_mutex);
        if(ring == nullptr){
            ring = new Ring<dbwg::LogInfo>(64);
        }
    }
    return ring;
}
//启动
void dbwg::LogStarter::start(){
    std::thread logThread(threadFunction);
    logThread.detach();
    logThreadisRunning = true;
    printf("[LogStarter::start] logwriter线程启动\n");
}
void dbwg::LogStarter::threadFunction(){
    printf("[LogStarter::threadFunction] logWriter开始\n");
    assert(!utils::mkdir("log",0777));

    //FILE *file = fopen("./log/log.txt","a");
    FILE *file = roll_file();
    int cur_size = 0;
    if(file == NULL){
        perror("Error opening file");
        return;
    }

    printf("[LogStarter::threadFunction] logwriter进入写循环\n");
    while(logThreadisRunning||!ring->isEmpty()){
        printf("[LogStarter::threadFunction] logwriter 加锁前\n");
        std::unique_lock<std::mutex> lock(logMutex);//写文件加锁
        cv.wait(lock, [&]{return !ring->isEmpty()|| !logThreadisRunning;});

        std::string tmp;
        while(!ring->isEmpty()){
            tmp += ring->pop().logFormat();
        }
        printf("[LogStarter::threadFunction] 写入 %s\n",tmp.c_str());
        if(!fprintf(file, "%s",tmp.c_str()))printf("Error writing to file.\n");
        fflush(file);

        //roll-file
        cur_size += tmp.size();
        if(cur_size > MAX_LOG_FILE_SIZE){
            fclose(file);
            file = roll_file();
            if(file == NULL){
                perror("[LogStarter::threadFunction] Error opening file");
                return;
            }
            cur_size = 0;
        }

        //printf("[LogStarter::threadFunction] 看看pop成功了吗\n head: %d \n tail: %d\n",ring->head,ring->tail);
    }
    fclose(file);
}
bool dbwg::LogStarter::isRunning(){
    return logThreadisRunning;
} 
FILE* dbwg::LogStarter::roll_file(){
    rp = (rp+1)%rfns.size();
    if(remove(rfns[rp].c_str())&&errno != ENOENT){
        perror("[roll_file] 文件删除失败");
        exit(0);
    }
    rfns[rp] = "./log/LOGFILE_" + std::to_string(rp) + "_" + dbwg::utils::now() + ".log";
    FILE* ret = fopen(rfns[rp].c_str(),"a");
    assert(ret != nullptr);
    return ret;
}

void dbwg::LogStarter::notify_one(){
    cv.notify_one();
}


//独立线程日志组件。
//启动器启动后，log("")即可。
void dbwg::log(std::string message,const char* file, int line,level::level levl){
    printf("[LogStarter::log] log开始\n");
    if(LogStarter::isRunning()){
        LogStarter::getRing()->push(LogInfo(message,levl,file,line));
        dbwg::LogStarter::notify_one();
    }
    else{
        printf("[LogStarter::log] Please execute \"dbwg::LogStarter::start\" ");
        exit(1);
    }
}
