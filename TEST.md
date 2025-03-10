# 性能测试：

（测试均包含创建日志目录，日志滚动文件的耗时。）

## 一、mmap测试： 2025.3.10

### 单线程百万日志

#### 异步

![image](https://github.com/user-attachments/assets/9b5fbe0c-5163-4342-9978-61f1c3d7d8ec)

4.9s

#### 同步

![image](https://github.com/user-attachments/assets/bdc56f54-52de-4f3c-a735-876bf794cfdd)

5.5s

这里每条都做msync了,去掉后能快几百ms

##### 单线程测试代码：
```
#include <iostream>
#include "dbwg.log.h"
#include <time.h>
#include <chrono>
using namespace dbwg;


#include "SyncLogger.hpp"
SyncLogger synclogger;
int main(){
    LOG("程序启动");
    int count = 1;

    
    auto start = std::chrono::high_resolution_clock::now();

    while(count <= 1000000){
        synclogger.log("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐",__FILE__,__LINE__,level::DEBUG);
    }


    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
    return 0;
}


// int main(){
//     LOG("程序启动");
//     int count = 1;

//     LogStarter::instance();
//     auto start = std::chrono::high_resolution_clock::now();

//     while(count <= 1000000){
//         debug_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         info_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         warn_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         error_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         fatal_str("模拟日志，日志序号"+std::to_string(count++)+"🤓🌹⭐");
//         debug("模拟日志，日志序号%d🤓🌹⭐",count++);
//         info("模拟日志，日志序号%d🤓🌹⭐",count++);
//         warn("模拟日志，日志序号%d🤓🌹⭐",count++);
//         error("模拟日志，日志序号%d🤓🌹⭐",count++);
//         fatal("模拟日志，日志序号%d🤓🌹⭐",count++);
//     }


//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
//     return 0;
// }

```

### 多线程百万日志

5个线程每个20万

#### 异步线程写

![image](https://github.com/user-attachments/assets/97e85ee1-4a52-4a2c-96cf-2aa35c3489c8)

6.69s （去掉msync为6.810s🤯）

#### 同步 （多线程用一个日志器，竞争锁写）

![image](https://github.com/user-attachments/assets/170c17d9-9662-455f-89d6-4bdc16bdcad2)

6.8s （去掉msync为6.058s）。

貌似效率差异并不大~

不同线程id:

![image](https://github.com/user-attachments/assets/1c1e3107-99f6-4995-803c-fddfac4e1ad8)

#### 多线程写 测试代码：

```
#include <iostream>
#include "dbwg.log.h"
#include <time.h>
#include <chrono>
using namespace dbwg;
#include <thread>
#include <vector>


void threadFunc(){
    for(int i=0;i<200000;i++){
        debug_str("模拟日志，日志序号"+std::to_string(i)+"🤓🌹⭐");
    }
}
int main(){
    LOG("程序启动");
    LogStarter::instance();

    auto start = std::chrono::high_resolution_clock::now();
    ////////////////
    
    int n = 5;
    std::vector<std::thread> threads;
    for(int i=0;i<n;i++){
        threads.emplace_back(threadFunc);
    }

    for(auto &thread :threads){
        thread.join();
    }

    ////////////////
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
    return 0;
}


// 多线程同步（用同一个日志器写）
// #include <thread>
// #include <vector>

// #include "SyncLogger.hpp"
// SyncLogger synclogger;

// void threadFunc(){
//     for(int i=0;i<200000;i++){
//         synclogger.log("模拟日志，日志序号"+std::to_string(i)+"🤓🌹⭐",__FILE__,__LINE__,level::DEBUG);
//     }
// }
// int main(){
//     LOG("程序启动");
//     auto start = std::chrono::high_resolution_clock::now();
//     ////////////////
    
//     int n = 5;
//     std::vector<std::thread> threads;
//     for(int i=0;i<n;i++){
//         threads.emplace_back(threadFunc);
//     }

//     for(auto &thread :threads){
//         thread.join();
//     }

//     ////////////////
//     auto end = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//     std::cout << "墙上时间: " << duration << " 毫秒" << std::endl;
//     return 0;
// }
```

---

## 二、旧版本对 fopen fwrite 的测试：

实际上当初没给同步封装，调用相同的方法。因此再没有尽可能控制变量的情况下，测试数据不适于比较，抱歉。

### 单线程百万日志

#### 本组件异步：

![image](https://github.com/user-attachments/assets/6540ab35-d970-4ad5-9eb3-168331f320f1)

**日志落地墙上时间**（实际经过的物理时间） 5s （第一个日志和最后一个日志相差）

主线程耗时 5.2s

#### 比较于同步写百万次日志：

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
