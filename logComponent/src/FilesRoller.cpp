#include "FilesRoller.hpp"

//滚动文件
//存文件名，一次只返回一个FILE*
namespace dbwg{

    FilesRoller::FilesRoller(int size)
        : rfns(std::vector<std::string>(size)),rp(0) {}

    UniqueFD FilesRoller::roll_file(){
        //下一个位置.先删除原先文件，然后新建文件
        rp = (rp+1)%rfns.size();
        if(rfns[rp].size()>0&&remove(rfns[rp].c_str())&&errno != ENOENT){
            perror("[roll_file] 文件删除失败");
            exit(1);
        }
        //文件名
        rfns[rp] = "./log/LOGFILE_" + std::to_string(rp) + " " + dbwg::utils::now() + ".log";
        UniqueFD fd = UniqueFD(open(rfns[rp].c_str(), O_RDWR | O_CREAT ,0644));
        if(fd.get() == -1){
            perror("[FilesRoller] file open failed\n");
        }
        return fd;
    }

}

    //文件指针版本
    // FILE* roll_file(){
    //     //下一个位置.先删除原先文件，然后新建文件
    //     rp = (rp+1)%rfns.size();
    //     if(rfns[rp].size()>0&&remove(rfns[rp].c_str())&&errno != ENOENT){
    //         perror("[roll_file] 文件删除失败");
    //         exit(1);
    //     }
    //     rfns[rp] = "./log/LOGFILE_" + std::to_string(rp) + " " + dbwg::utils::now() + ".log";
    //     FILE* ret = fopen(rfns[rp].c_str(),"a");//打开一个文件，外部可用智能指针来fclose
    //     if(ret == nullptr){
    //         perror("[FilesRoller] file open error\n");
    //     }
    //     return ret;
    // }

