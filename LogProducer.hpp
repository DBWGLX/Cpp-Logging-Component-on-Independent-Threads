#ifndef DBWG_LOGPRODUCER
#define DBWG_LOGPRODUCER
#include <iostream>
#include <mutex>
#include <queue>

namespace dbwg{
    //任务队列
    template<class T>
    class LogProducer{
        public:
            void push(T aim){
                std::lock_guard<std::mutex>guard(_mutex);
                _queue.push(aim);
            }
            T pop(){
                std::lock_guard<std::mutex>guard(_mutex);
                if(_queue.size()<=0){
                    perror("[LogProducer][ERROR] 队列空仍pop\n");
                    return {};
                }else{
                    T ret = _queue.front();
                    _queue.pop();
                    return ret;
                }
            }
            size_t size(){
                //没加锁，动态值获取大概即可
                std::lock_guard<std::mutex>guard(_mutex);
                return _queue.size();   
            }
        private:
            std::queue<T>_queue;
            std::mutex _mutex;
    };
}

#endif