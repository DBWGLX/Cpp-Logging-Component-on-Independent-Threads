#ifndef UNIQUEFD
#define UNIQUEFD
#include <unistd.h>

namespace dbwg{
    class UniqueFD{
    public:
        explicit UniqueFD(int fd = -1);
        ~UniqueFD();
        //禁止拷贝
        UniqueFD(const UniqueFD&) = delete;
        UniqueFD& operator=(const UniqueFD&) = delete;
        //支持移动
        UniqueFD(UniqueFD&& other) noexcept;
        UniqueFD& operator=(UniqueFD&& other)noexcept;
        int get() const;
        void reset(int new_fd = -1);
        void reset(UniqueFD&& other);
        int release();

    private:
        int fd_;
    };
}

#endif