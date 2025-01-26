dbwg::LogWriter::LogWriter(int ringSize)
    :head(0),tail(0),running(true){
    ring.resize(ringSize);
}
void dbwg::LogWriter::logWriter(){
    printf("logWriter开始\n");
    assert(!utils::mkdir("log",0777));
    FILE *file = fopen("log.txt","a");
    if(file == NULL){
        perror("Error opening file");
        return;
    }
    printf("logwriter进入写循环\n");
    while(running||!ringEmpty()){
        std::unique_lock<std::mutex> lock(logMutex);//写文件加锁
        logCondition.wait(lock, [&]{return !ringEmpty()|| !running;});
        std::string tmp;
        while(!ringEmpty()){
            tmp += pop();
        }
        fprintf(file, "%s",tmp.c_str());
    }
    fclose(file);
}
void dbwg::LogWriter::push(LogInfo loginfo){
    while((tail+1)%ring.size() == head){
        perror("log buffer is full");
        sleep(1000);
    }
    ring[tail] = loginfo;
    tail = (tail+1)%ring.size(); 
    logCondition.notify_one();
    printf("    ring get a push\n");
}
std::string dbwg::LogWriter::pop(){
    std::string ret = ring[head].logFormat();
    head = (head+1)%ring.size();
    return ret;
}
bool dbwg::LogWriter::ringEmpty(){
    return head == tail;
}
void dbwg::LogWriter::stopLogging(){
    running = false;
    logCondition.notify_one();
}

// LogStarter 静态成员变量定义
dbwg::LogWriter* dbwg::LogStarter::logWriter = nullptr;
std::mutex dbwg::LogStarter::lwmutex;
bool dbwg::LogStarter::logThreadRunning = false;

dbwg::LogWriter* dbwg::LogStarter::getLogWriter(){
    if(logWriter == nullptr){
        //lwmutex.lock();
        std::lock_guard<std::mutex> lock(lwmutex);
        if(logWriter == nullptr){
            logWriter = new LogWriter();
        }
        //lwmutex.unlock();
    }
    return logWriter;
}