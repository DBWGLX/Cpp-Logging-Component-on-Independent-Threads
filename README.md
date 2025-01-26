# 独立线程异步写日志组件

### 日志效果

时间-等级-线程-文件-行号-内容

![image](https://github.com/user-attachments/assets/82811a28-05f2-4e0f-a234-e2de0ac9b4df)

---

### 滚动日志文件效果 roll log file effect

![9c185e1d-55cb-4adc-b97f-4ffc61545d4f](https://github.com/user-attachments/assets/e76cb8a7-5dfa-4fb6-8202-45de57632a14)

这里设置每次启动从1开始，10个后会删除第一个并且新日志文件作为第一个，以此类推：

![095d7be9-9c58-4b08-93cf-459348c73d11](https://github.com/user-attachments/assets/b74c6c13-7078-4a9b-912c-d828853bcd94)


---

### 使用手册

`#include "dbwg.log.h"` 即可使用

1. `LOG(fmt, ...)` 可以向终端打印信息格式化信息

2. 程序开始时执行 `dbwg::LogStarter::start();` 后启动一个独立线程用于写日志。<br>
通过调用 `LOG1(message)` 可以把字符串message日志打印到 log 目录下。<br>
日志文件自动创建，文件名为创建时间。文件为滚动文件的形式。<br>
单个文件默认大小在logSink.cc里设置宏 MAX_LOG_FILE_SIZE。<br>
默认10个文件，第十一个会覆盖第一个，以此类推。<br>


---

### 项目结构介绍

- **dbwg.log.h** 总文件，会包含其他子文件，使用本项目时只需包含这个文件即可使用。
- **logInfo.hpp** 日志信息类
- **logSink.hpp** 日志落地类
- **level.cpp** 日志等级
- **ring.hpp** 缓冲区，环形队列
- **utils.cpp** 工具函数


