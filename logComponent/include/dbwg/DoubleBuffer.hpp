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
        DoubleBuffer(size_t size = 2048);
        ~DoubleBuffer() = default;
        std::shared_ptr<char[]> getBuffer1();
        std::shared_ptr<char[]> getBuffer2();
        void swap();
        bool isBuffer1Full();
        size_t size();
        void setBuffer1Full();
        int get_index();
        void addIndex(int aim);
    private:
        std::shared_ptr<char[]> buffers[2];
        std::atomic<int> producer_idx;//生产者下标
        size_t _size;//每个buff的大小
        bool buff1IsFull;
        int cur_index;//可以用来作为下标
    };
}

#endif