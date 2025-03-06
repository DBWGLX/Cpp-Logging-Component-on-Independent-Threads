#ifndef DBWGLX_LOG_BUFFER
#define DBWGLX_LOG_BUFFER
#include <iostream>
#include <memory>
#include <atomic>

//生产者消费者双缓冲区
//生产者生产buff1，消费者处理buff2。待消费者处理完，生产者生产满，做一次交换
namespace dbwg{
    class DoubleBuffer{
    public:
        DoubleBuffer(size_t size = 2048)
            : producer_idx(0),
              _size(size)
        {
            //在 C++17 及以后的标准中，std::make_shared 不能直接用于创建 std::shared_ptr<char[]> 这种动态数组类型的智能指针，因为 std::make_shared 主要设计用于创建单个对象，而不是数组。
            //内存释放时会使用delete而非delete[]，造成未定义行为（如double free或堆损坏）。
            buffers[0] = std::shared_ptr<char[]>(new char[size], std::default_delete<char[]>());
            buffers[1] = std::shared_ptr<char[]>(new char[size], std::default_delete<char[]>());
            clearBuffer2();
        }
        ~DoubleBuffer() = default;
        std::shared_ptr<char[]> getBuffer1(){
            return buffers[producer_idx.load()];
        }
        std::shared_ptr<char[]> getBuffer2(){
            return buffers[1-producer_idx.load()];
        }
        void swap(){
            producer_idx.store(1-producer_idx.load());
            buff2IsClear = 0;
        }
        void clearBuffer2(){
            if(getBuffer2().get()){
                getBuffer2().get()[0] = '\0';
                buff2IsClear = 1;
            }else{
                perror("[DoubleBuffer] buff2 error\n");
            }
        }
        //bool isBuffer2Clear(){
        bool isBuffer2Clear(){
            if(buffers[1-producer_idx.load()].get()){
                return buff2IsClear;
            }else{
                perror("[DoubleBuffer::buffer2IsClear] buff2 error\n");
            }
            return false;
        }
        size_t size(){
            return _size;
        }
    private:
        std::shared_ptr<char[]> buffers[2];
        std::atomic<int> producer_idx;
        size_t _size;
        bool buff2IsClear;
    };
}

#endif