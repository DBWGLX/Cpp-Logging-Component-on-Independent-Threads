#ifndef DBWGLX_LOG_ROLLFILE
#define DBWGLX_LOG_ROLLFILE
#include <iostream>
#include <cassert>
#include "utils.hpp"


//滚动文件
//存文件名，一次只返回一个FILE*
namespace dbwg{
    class FilesRoller(){
    public:
        RollFile(int size = 10)
            : rfns(vector<int>(size)),rp(0) {}

        FILE* dbwg::LogStarter::roll_file(){
            //先删除下一个位置的原先文件，然后新建文件
            rp = (rp+1)%rfns.size();
            if(remove(rfns[rp].c_str())&&errno != ENOENT){
                perror("[roll_file] 文件删除失败");
                exit(0);
            }
            rfns[rp] = "./log/LOGFILE_" + dbwg::utils::now() + ".log";
            FILE* ret = fopen(rfns[rp].c_str(),"a");
            assert(ret != nullptr);
            return ret;
        }
    private:
        std::vector<std::string>rfns;//roll-file-names
        int rp;//rfns' current index
    }
}

#endif