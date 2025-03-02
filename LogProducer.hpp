#ifndef DBWG_LOGPRODUCER
#define DBWG_LOGPRODUCER
#include <iostream>
#include <mutex>
#include <queue>

namespace dbwg{
    template<class T>
    class LogProducer{
        public:
            void push(T aim){
                std::lock_guard<std::mutex>guard(_mutex);
                _queue.push(aim);
            }
            T pop(){
                T ret = _queue.front();
                _queue.pop();
                return ret;
            }
            size_t size(){
                //没加锁，一般是动态值，获取大概即可
                return _queue.size();   
            }
        private:
            std::queue<T>_queue;
            std::mutex _mutex;
    };
}

#endif