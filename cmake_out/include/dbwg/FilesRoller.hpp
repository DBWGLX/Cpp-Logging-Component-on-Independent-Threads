#ifndef DBWGLX_LOG_ROLLFILE
#define DBWGLX_LOG_ROLLFILE
#include <iostream>
#include <vector>
#include <cassert>
#include <fcntl.h>
#include "utils.hpp"
#include "UniqueFD.hpp"

//滚动文件
//存文件名，一次只返回一个FILE*
namespace dbwg{
    class FilesRoller{
    public:
        FilesRoller(int size = 16);
        UniqueFD roll_file();
    private:
        std::vector<std::string>rfns;//roll-file-names
        int rp;//rfns' current index
    };
}

#endif