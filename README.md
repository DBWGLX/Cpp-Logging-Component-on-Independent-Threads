![REPO SIZE](https://img.shields.io/github/repo-size/DBWGLX/Cpp-Logging-Component-on-Independent-Threads.svg)
![CODE SIZE](https://img.shields.io/github/languages/code-size/DBWGLX/Cpp-Logging-Component-on-Independent-Threads.svg)
![License](https://img.shields.io/github/license/DBWGLX/Cpp-Logging-Component-on-Independent-Threads.svg)

# 独立线程异步写日志组件

### 简介

#### 日志效果

时间-等级-进程id-线程id-文件-行号-内容

![image](https://github.com/user-attachments/assets/82811a28-05f2-4e0f-a234-e2de0ac9b4df)

#### 性能测试见TEST.md

#### 滚动日志文件效果 roll log file effect

![9c185e1d-55cb-4adc-b97f-4ffc61545d4f](https://github.com/user-attachments/assets/e76cb8a7-5dfa-4fb6-8202-45de57632a14)

这里设置每次启动从1开始，10个后会删除第一个并且新日志文件作为第一个，以此类推：

![095d7be9-9c58-4b08-93cf-459348c73d11](https://github.com/user-attachments/assets/b74c6c13-7078-4a9b-912c-d828853bcd94)

日志文件在类内部自动创建，文件名为序号+创建时间（如上）。滚动文件在单词执行文件创建达到上限后自动删除最旧日志。<br>

---

### 使用手册

`#include "dbwg_logger.hpp"` 并且正确链接动态库，即可使用。具体可参考prompt文件。

#### 配置

在异步日志器类LogStarter.hpp和SyncLogger.hpp的宏中可修改。

双缓冲区每个缓冲区的大小，滚动日志文件数目上限，日志文件大小<br>
默认 1e5 字节缓冲区，1024 个文件，日志文件一百万字节。<br>

#### 3钟输出方式：

1.标准输出打印

执行 `LOG(fmt, ...)` 可以向终端打印信息格式化信息。

这是一个宏，2也是，具体见dbwg_logger.hpp

2.异步线程写滚动文件

以下宏代表日志等级

debug(str, ...)/info()/warn()/error()/fatal() 可以把c可变参数格式日志打印到 log 目录下。

debug_str(str)/info_str()/warn_str()/error_str()/fatal_str() 适配std::string

调用后，会执行类LogStarter的静态方法instance()获取该日志器单例执行log方法。首次调用执行构造，此时会构建异步日志线程。

3.同步写滚动文件

此为互斥同步写，即便是多线程也可以正常写入。

需要SyncLogger类实例，如SyncLogger synclogger;

调用其log方法，如：`synclogger.log("模拟日志，日志序号"+std::to_string(i)+"🤓🌹⭐",__FILE__,__LINE__,level::DEBUG);`

此为个人做性能测试编写。


---

### 项目结构介绍

- **dbwg_logger.hpp** 总头文件，会包含其他子文件，使用本项目时只需包含这个文件即可使用。
- **utils.cpp** 工具函数
- **LogInfo.hpp** 日志信息类
- **Level.hpp** 日志等级枚举
- **DoubleBuffer.hpp** 双缓冲区
- **FilesRoller.hpp** 滚动文件设计
- **UniqueFD.hpp** 智能文件描述符
- **LogStarter.hpp** 异步日志器类
- **SyncLogger.hpp** 同步日志器类


