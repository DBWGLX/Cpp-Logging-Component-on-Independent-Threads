![REPO SIZE](https://img.shields.io/github/repo-size/DBWGLX/Cpp-Logging-Component-on-Independent-Threads.svg)
![CODE SIZE](https://img.shields.io/github/languages/code-size/DBWGLX/Cpp-Logging-Component-on-Independent-Threads.svg)
![License](https://img.shields.io/github/license/DBWGLX/Cpp-Logging-Component-on-Independent-Threads.svg)

# 独立线程异步写日志组件

### 日志效果

时间-等级-线程-文件-行号-内容

![image](https://github.com/user-attachments/assets/82811a28-05f2-4e0f-a234-e2de0ac9b4df)

性能见TEST.md

---

### 滚动日志文件效果 roll log file effect

![9c185e1d-55cb-4adc-b97f-4ffc61545d4f](https://github.com/user-attachments/assets/e76cb8a7-5dfa-4fb6-8202-45de57632a14)

这里设置每次启动从1开始，10个后会删除第一个并且新日志文件作为第一个，以此类推：

![095d7be9-9c58-4b08-93cf-459348c73d11](https://github.com/user-attachments/assets/b74c6c13-7078-4a9b-912c-d828853bcd94)


---

### 使用手册

`#include "dbwg.log.h"` 即可使用

1. `LOG(fmt, ...)` 可以向终端打印信息格式化信息

debug()/info()/warn()/error()/fatal() 可以把c可变参数格式日志打印到 log 目录下。

debug_str()/info_str()/warn_str()/error_str()/fatal_str() 适配std::string

2. 日志文件自动创建，文件名为创建时间。文件为滚动文件的形式。<br>
双缓冲区的每个大小，日志文件数目上限，日志文件大小默认大小在**logStarter.hpp**里设置宏即可调整。<br>
默认 1e5 字节缓冲区，1024个文件，日志文件一百万字节。程序每次运行会重新计数日志数目，达到上限后，最新的会覆盖最旧的。<br>


---

### 项目结构介绍

- **dbwg.log.h** 总文件，会包含其他子文件，使用本项目时只需包含这个文件即可使用。
- **LogInfo.hpp** 日志信息类
- **Level.hpp** 日志等级
- **LogStarter.hpp** 日志落地类
- **DoubleBuffer.hpp** 双缓冲区
- **FilesRoller.hpp** 滚动文件设计
- **utils.cpp** 工具函数


