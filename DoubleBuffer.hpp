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
        DoubleBuffer(size_t size = 1024)
            : producer_idx(0),
              _size(size)
        {
            buffers[0] = std::make_shared<char[]>(size),
            buffers[1] = std::make_shared<char[]>(size),
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
            printf("[DoubleBuffer] swapping, p_idx: %d\n",producer_idx.load());
            producer_idx.store(1-producer_idx.load());
            buff2IsClear = 0;
            printf("[DoubleBuffer] swapping down, p_idx: %d\n",producer_idx.load());
        }
        void clearBuffer2(){
            if(getBuffer2().get()){
                getBuffer2().get()[0] = '\0';
                buff2IsClear = 1;
            }else{
                printf("[DoubleBuffer] buff2 error\n");
            }
            printf("[DoubleBuffer::clearBuffer2] buff2IsClear: %d\n",buff2IsClear);
        }
        //bool isBuffer2Clear(){
        bool isBuffer2Clear(){
            printf("[DoubleBuffer] buff2IsClear: %d\n",buff2IsClear);
            if(buffers[1-producer_idx.load()].get()){
                return buff2IsClear;
            }else{
                printf("[DoubleBuffer::buffer2IsClear] buff2 error\n");
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