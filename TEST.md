# 性能测试：

### 单线程调用十万次日志

#### 本组件异步：

![image](https://github.com/user-attachments/assets/36719175-68f5-4926-820b-3ce02c34c762)

**墙上时间**（实际经过的物理时间） 6s （第一个日志和最后一个日志相差）

clock() 函数测量的是 **CPU 时间**（所有线程的总耗时） 10.85 s

可以认为主线程耗时4s左右。

实际上，线程的创建等，也有一定的时间开销.而且本组件是按双缓冲区写时，及时fflush缓冲区的，尽可能增加写的次数避免日志丢失。

本组件测试代码：
```
#include <iostream>
#include "dbwg.log.h"
#include<time.h>
using namespace dbwg;


int main(){
    LOG("程序启动");
    int count = 1;
    clock_t ct = clock();

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
    
    ct = clock() -ct;
    std::cout << ct << std::endl;
    return 0;
}
```

#### 比较于同步写十万次日志：

![image](https://github.com/user-attachments/assets/c522d8a4-f112-442e-ba18-76e5540480c6)

同步只耗时3.36s。

同步测试代码：
```
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

