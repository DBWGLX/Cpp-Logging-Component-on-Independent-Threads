#include "UniqueFD.hpp"
namespace dbwg{

    UniqueFD::UniqueFD(int fd):fd_(fd){
    }

    UniqueFD::~UniqueFD(){
        if(fd_ != -1){
            close(fd_);
        }
    }

    //支持移动
    UniqueFD::UniqueFD(UniqueFD&& other)noexcept: fd_(other.fd_){
        other.fd_ = -1;
    }

    UniqueFD& UniqueFD::operator=(UniqueFD&& other)noexcept{
        if(this != &other){
            if(fd_ != -1) close(fd_);
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }

    int UniqueFD::get()const{return fd_;}

    void UniqueFD::reset(int new_fd){
        if(fd_ != -1)close(fd_);
        fd_ = new_fd;
    }

    void UniqueFD::reset(UniqueFD&& other){
        if(this != &other){
            reset();
            fd_ = other.fd_;
            other.fd_ = -1;
        }
    }

    int UniqueFD::release(){
        int old_fd = fd_;
        fd_ = -1;
        return old_fd;
    }
}
