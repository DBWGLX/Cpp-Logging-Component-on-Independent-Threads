# 性能测试：

（测试均包含日志目录，日志滚动文件的创建耗时。）

### 单线程调用十万次日志

#### 本组件异步：

![image](https://github.com/user-attachments/assets/6540ab35-d970-4ad5-9eb3-168331f320f1)

**日志落地墙上时间**（实际经过的物理时间） 5s （第一个日志和最后一个日志相差）

主线程耗时 5.2s

#### 比较于同步写十万次日志：

![image](https://github.com/user-attachments/assets/e6235730-4fe9-43c2-80b4-f3f5185f730e)

同步耗时4s:

![image](https://github.com/user-attachments/assets/3772e9a3-79ee-4f79-b1cd-9097473a2960)

如果每次都刷新缓冲区，耗时6s，频繁的IO带来的性能损耗其实没有想象中的大。

可能是因为磁盘性能足够好（SSD），以及系统层面的文件缓存（Page Cache）或是标准库缓冲帮忙吸收磁盘写入压力了。

测试代码：
```
#include <iostream>
#include "dbwg.log.h"
#include<time.h>
using namespace dbwg;
#include <chrono>

int main(){
    LOG("程序启动");
    int count = 1;

    LogStarter::instance();
    auto start = std::chrono::high_resolution_clock::now();

    while(count <= 1000000){
        debug_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        info_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        warn_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        error_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        fatal_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
        debug("模拟日志，日志序号%d🤓🌹⭐",count++);
        info("模拟日志，日志序号%d🤓🌹⭐",count++);
        warn("模拟日志，日志序号%d🤓🌹⭐",count++);
        error("模拟日志，日志序号%d🤓🌹⭐",count++);
        fatal("模拟日志，日志序号%d🤓🌹⭐",count++);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
    return 0;
}


// 同步
#include <iostream>
#include "dbwg.log.h"
#include<time.h>
using namespace dbwg;
#include <chrono>

int main(){
    LOG("程序启动");
    int count = 1;

    LogStarter::instance();
    auto start = std::chrono::high_resolution_clock::now();

    //创建log目录
    if (utils::mkdir("log", 0777) != 0) {//我的mkdir，即使存在也返回0
        perror("Error creating log directory");
        return 1;
    }
    FilesRoller files_roller;
    FILE*file = files_roller.roll_file();
    while(count <= 1000000){
        int rs = fprintf(file, "%s [%s:%d] " "模拟日志：%d" "\n",dbwg::utils::now().c_str(),__FILE__,__LINE__,count++);
        if(rs<0)perror("[threadFunction_consumer] Error writing to file.\n");
        if(count%111111 == 0){
            file = files_roller.roll_file();
        }
        if(count % 100000 == 0){
            fflush(file);
        }
    }
    fflush(file);


    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
    return 0;
}
```

### 多线程同时写


