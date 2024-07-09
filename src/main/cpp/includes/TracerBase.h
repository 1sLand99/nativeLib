//
// Created by Zhenxi on 2024/7/8.
//

#ifndef RUNTIME_TRACERBASE_H
#define RUNTIME_TRACERBASE_H

#include <iosfwd>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <jni.h>
#include <dlfcn.h>
#include <cstddef>
#include <fcntl.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sstream>
#include <ostream>

#include <cstdlib>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <climits>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <pthread.h>
#include <vector>
#include <zlib.h>
#include <list>
#include <string>
#include <map>
#include <list>

#include <cstring>
#include <cstdio>
#include <regex>
#include <cerrno>
#include <climits>
#include <iostream>
#include <fstream>

#include "ITracerBase.h"



#define  ORIG_BUFF_SIZE  100
#define  MATCH_SO_NAME_SIZE  100
#define  MAX_PRINTF_SIZE  PATH_MAX

//#define INIT_ORIG_BUFF \
//    char *buff = (char*)malloc(ORIG_BUFF_SIZE); \
//    if(buff == NULL){return ret; }                   \
//    my_memset(buff,0,ORIG_BUFF_SIZE); \
//
//# define APPEND(buff, value) \
//     if(my_strlen(value)>0){     \
//        buff = (char *)realloc(buff,my_strlen(buff)+my_strlen(value)+1); \
//        if(buff == NULL){ return ret; }                     \
//        my_strcat(buff,value);  \
//     }  \



#define INIT_ORIG_BUFF string buff;

#define APPEND(buff, value) buff.append(value);






//这块是为了防止他把一个void*当成char*去strlen挂掉
# define IS_NULL(value) value \

# define APPEND_INT(buff, value) \
     char* int_value = int_to_str(value);           \
     if(int_value!=nullptr){                           \
        APPEND(buff,int_value);                   \
        free(int_value);                           \
     }


/**
 * address = 符号地址-基地址
 */
#define GET_ADDRESS \
 void* address_offset = (void*)((char *)info.dli_saddr - (size_t) info.dli_fbase);                   \
 auto address = TracerBase::getAddressHex(address_offset);\



# define CLEAN_APPEND(buff, value, size) \
     my_memset(buff,0,size); \
     my_strcat(buff,value);   \

# define DL_INFO \
    Dl_info info={0}; \
    int addr_ret_0 = dladdr((void *) __builtin_return_address(0), &info); \

# define IS_MATCH \
        if(TracerBase::isLister(addr_ret_0,&info)){ \

# define WRITE TracerBase::write \



class TracerBase : public ITracerBase{
public:

    virtual void init(JNIEnv *env,
              bool hookAll,
              const std::list<std::string> &forbid_list,
              const std::list<std::string> &filter_list,
              std::ofstream *os) = 0;

    virtual void stop() = 0;

    static bool isHookAll;
    static std::ofstream *traceOs;
    static std::list<std::string> filterSoList;
    static std::list<std::string> forbidSoList;
    static bool isSave;
    static std::string match_so_name;


    /**
      * 是否监听第二级返回结果。
    */
    static bool isHookSecondRet;
    static void write(char *msg, Dl_info info);
    static void write(const char *msg, Dl_info info);
    static void write(const std::string &msg, Dl_info info);
    static void write(const std::wstring &msg, Dl_info info);
    static void write(const std::wstring *msg, Dl_info info);
    static void write(const std::string &msg);
    static void write(const std::string &msg, [[maybe_unused]] bool isApart);
    static void write(std::string &msg, Dl_info info);
    static char *getAddressHex(void *ptr);

    static bool isLister(int dladd_ret, Dl_info *info);

    static bool isAppFile(const char *path);

    static std::string getFileNameForPath(const char *path);

private:
    static void write_inline(const char* msg,Dl_info info);
    static void write_inline(const char* msg,Dl_info info,bool isApart);

};


#endif //RUNTIME_TRACERBASE_H
