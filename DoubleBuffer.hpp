#ifndef DBWGLX_LOG_BUFFER
#define DBWGLX_LOG_BUFFER
#include <iostream>
#include <memory>

//生产者消费者双缓冲区
//生产者生产buff1，消费者处理buff2。待消费者处理完，生产者生产满，做一次交换
namespace dbwg{
    class DoubleBuffer{
    public:
        DoubleBuffer(size_t size = 1024)
            : buff1(std::make_shared<char[]>(size)),
              buff2(std::make_shared<char[]>(size)),
              _size(size){}
        ~DoubleBuffer() = default;
        std::shared_ptr<char[]> getBuffer1(){
            return buff1;
        }
        std::shared_ptr<char[]> getBuffer2(){
            return buff2;
        }
        void swap(){
            std::swap(buff1,buff2);
        }
        void clearBuffer2(){
            if(buff2){
                buff2.get()[0] = '\0';
            }
        }
        size_t size(){
            return _size;
        }
    private:
        std::shared_ptr<char[]> buff1;//生产者
        std::shared_ptr<char[]> buff2;//消费者
        size_t _size;
    };
}

#endif