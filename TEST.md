# 性能测试：

（测试均包含日志目录，日志滚动文件的创建耗时。）

### 单线程调用十万次日志

#### 本组件异步：

![image](https://github.com/user-attachments/assets/2a549505-a6c7-4cb6-b3a2-5b431247191c)

**日志落地墙上时间**（实际经过的物理时间） 10s （第一个日志和最后一个日志相差）

主线程耗时 5.8s

本组件是按双缓冲区写，及时fflush缓冲区的，尽可能增加写的次数避免意外丢失日志。

#### 比较于同步写十万次日志：

![image](https://github.com/user-attachments/assets/c522d8a4-f112-442e-ba18-76e5540480c6)

同步耗时3.36s。这里的同步写操作系统的缓存区，由操作系统来调度的，刷新缓冲区次数最少。


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


int main(){
    LOG("程序启动");
    int count = 1;

    clock_t ct = clock();
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
    }
    fflush(file);
    ct = clock() -ct;
    
    std::cout << ct << std::endl;
    return 0;
}
```

