#include "level.hpp"

std::string dbwg::level::level2str(level levl){
    switch(levl){
        case OFF:   return "OFF";
        case FATAL: return "FATAL";
        case ERROR: return "ERROR";
        case WARN:  return "WARN";
        case INFO:  return "INFO";
        case DEBUG: return "DEBUG";
        default: 
            perror("error parameter of leve2str");
    };
    return "UNKNOWN";
}   