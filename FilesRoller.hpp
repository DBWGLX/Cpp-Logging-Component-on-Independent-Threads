#ifndef DBWGLX_LOG_ROLLFILE
#define DBWGLX_LOG_ROLLFILE
#include <iostream>
#include <vector>
#include <cassert>
#include "utils.hpp"


//滚动文件
//存文件名，一次只返回一个FILE*
namespace dbwg{
    class FilesRoller{
    public:
        FilesRoller(int size = 10)
            : rfns(std::vector<std::string>(size)),rp(0) {}

        FILE* roll_file(){
            //先删除下一个位置的原先文件，然后新建文件
            rp = (rp+1)%rfns.size();
            if(rfns[rp].size()>0&&remove(rfns[rp].c_str())&&errno != ENOENT){
                perror("[roll_file] 文件删除失败");
                exit(0);
            }
            rfns[rp] = "./log/LOGFILE " + dbwg::utils::now() + ".log";
            FILE* ret = fopen(rfns[rp].c_str(),"a");
            if(ret == nullptr){
                perror("[FilesRoller] file open \n");
            }
            return ret;
        }
    private:
        std::vector<std::string>rfns;//roll-file-names
        int rp;//rfns' current index
    };
}

#endif