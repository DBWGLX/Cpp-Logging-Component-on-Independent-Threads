#ifndef UNIQUEFD
#define UNIQUEFD
#include <unistd.h>

namespace dbwg{
    class UniqueFD{
    public:
        explicit UniqueFD(int fd = -1):fd_(fd){}

        ~UniqueFD(){
            if(fd_ != -1){
                close(fd_);
            }
        }

        //禁止拷贝
        UniqueFD(const UniqueFD&) = delete;
        UniqueFD& operator=(const UniqueFD&) = delete;

        //支持移动
        UniqueFD(UniqueFD&& other) noexcept : fd_(other.fd_){
            other.fd_ = -1;
        }

        UniqueFD& operator=(UniqueFD&& other)noexcept{
            if(this != &other){
                if(fd_ != -1) close(fd_);
                fd_ = other.fd_;
                other.fd_ = -1;
            }
            return *this;
        }

        int get() const{return fd_;}

        void reset(int new_fd = -1){
            if(fd_ != -1)close(fd_);
            fd_ = new_fd;
        }

        void reset(UniqueFD&& other){
            if(this != &other){
                reset();
                fd_ = other.fd_;
                other.fd_ = -1;
            }

        }

        int release(){
            int old_fd = fd_;
            fd_ = -1;
            return old_fd;
        }

    private:
        int fd_;
    };
}

#endif