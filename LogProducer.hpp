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
                std::cout << "【DEBUG】Pushing element with address: " << &aim << std::endl;
                printf("[push] %s",aim.logFormat().c_str());
                _queue.push(aim);
            }
            T pop(){
                printf("【DEBUG】pop\n");
                std::lock_guard<std::mutex>guard(_mutex);
                if(_queue.size()<=0){
                    perror("[LogProducer][ERROR] 队列空仍pop\n");
                    return {};
                }else{
                    T ret = _queue.front();
                    std::cout << "【DEBUG】Popping element with address: " << &ret << std::endl;
                    printf("【DEBUG】成功获取front: %s，queue.size(): %d\n",ret.logFormat().c_str(),_queue.size());
                    _queue.pop();
                    printf("【DEBUG】成功 pop\n");
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