#include "DoubleBuffer.hpp"

//生产者消费者双缓冲区
//生产者生产buff1，消费者处理buff2。待消费者处理完，生产者生产满，做一次交换
namespace dbwg{
    DoubleBuffer::DoubleBuffer(size_t size)
        : producer_idx(0),
            _size(size),
            cur_index(0),
            buff1IsFull(0)
    {
        //在 C++17 及以后的标准中，std::make_shared 不能直接用于创建 std::shared_ptr<char[]> 这种动态数组类型的智能指针，因为 std::make_shared 主要设计用于创建单个对象，而不是数组。
        //内存释放时会使用delete而非delete[]，造成未定义行为（如double free或堆损坏）。
        buffers[0] = std::shared_ptr<char[]>(new char[size], std::default_delete<char[]>());
        buffers[1] = std::shared_ptr<char[]>(new char[size], std::default_delete<char[]>());
    }
    std::shared_ptr<char[]> DoubleBuffer::getBuffer1(){
        return buffers[producer_idx.load()];
    }
    std::shared_ptr<char[]> DoubleBuffer::getBuffer2(){
        return buffers[1-producer_idx.load()];
    }
    void DoubleBuffer::swap(){
        producer_idx.store(1-producer_idx.load());
        buff1IsFull = 0;
        cur_index = 0;
    }
    bool DoubleBuffer::isBuffer1Full(){
        if(buffers[1-producer_idx.load()].get()){
            return buff1IsFull;
        }else{
            perror("[DoubleBuffer::isBuffer1Full] buff1 error\n");
        }
        return false;
    }
    size_t DoubleBuffer::size(){
        return _size;
    }
    void DoubleBuffer::setBuffer1Full(){
        buff1IsFull = true;
    }
    int DoubleBuffer::get_index(){
        return cur_index;
    }
    void DoubleBuffer::addIndex(int aim){
        cur_index += aim;
    }
}
