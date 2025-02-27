#ifndef DBWGLX_RING
#define DBWGLX_RING
#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#endif
namespace dbwg{
    template<typename T>
    class Ring{
    public:
        // 环形队列
        // _ _ head _ _ _ tail _ _
        // tail指向下一个空位置
        // 这里tail永远为空
        Ring(int ringSize = 64){
            ring.resize(ringSize);
            head = tail = 0;
        }
        int push(T member){
            std::lock_guard<std::mutex> lock(mutex_push);

            while(isFull()){
                perror("[ring::push] ring is full!");
                printf("head:%d tail:%d\n",head,tail);
                sleep(1);
            }
            ring[tail] = member;
            tail = (tail+1)%ring.size();
            return 0;
        }
        T pop(){
            std::lock_guard<std::mutex> lock(mutex_pop);

            while(isEmpty()){
                perror("[ring::pop] ring is empty!");
                printf("head:%d tail:%d\n",head,tail);
                sleep(1);
            }
            T ret = ring[head];
            head = (head+1)%ring.size();
            return ret;
        }
        bool isFull(){
            return (tail+1)%ring.size() == head;
        }
        bool isEmpty(){
            return head == tail;
        }
    private:
        int head,tail;
        std::vector<T> ring;
        std::mutex mutex_push;
        std::mutex mutex_pop;
        std::condition_variable cdtvrble;
    };
}