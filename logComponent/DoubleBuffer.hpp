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
              _size(size),
              cur_index(0)
        {
            //在 C++17 及以后的标准中，std::make_shared 不能直接用于创建 std::shared_ptr<char[]> 这种动态数组类型的智能指针，因为 std::make_shared 主要设计用于创建单个对象，而不是数组。
            //内存释放时会使用delete而非delete[]，造成未定义行为（如double free或堆损坏）。
            buffers[0] = std::shared_ptr<char[]>(new char[size], std::default_delete<char[]>());
            buffers[1] = std::shared_ptr<char[]>(new char[size], std::default_delete<char[]>());
            clearBuffer1();
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
            buff1IsClear = 0;
            cur_index = 0;
        }
        void clearBuffer1(){
            if(getBuffer1().get()){
                getBuffer1().get()[0] = '\0';
                buff1IsClear = 1;
            }else{
                perror("[DoubleBuffer::clearBuffer1] buff1 error\n");
            }
        }
        //bool isBuffer2Clear(){
        bool isBuffer1Full(){
            if(buffers[1-producer_idx.load()].get()){
                return buff1IsClear;
            }else{
                perror("[DoubleBuffer::isBuffer1Full] buff1 error\n");
            }
            return false;
        }
        size_t size(){
            return _size;
        }
        int get_index(){
            return cur_index;
        }
        void addIndex(int aim){
            cur_index += aim;
        }
    private:
        std::shared_ptr<char[]> buffers[2];
        std::atomic<int> producer_idx;//生产者下标
        size_t _size;//每个buff的大小
        bool buff1IsClear;
        int cur_index;//可以用来作为下标
    };
}

#endif