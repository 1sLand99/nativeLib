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

#define GET_RETURN_ADDRESS(depth) \
    ((depth) == 0 ? __builtin_return_address(0) : \
    (depth) == 1 ? __builtin_return_address(1) : \
    (depth) == 2 ? __builtin_return_address(2) : \
    (depth) == 3 ? __builtin_return_address(3) : \
    (depth) == 4 ? __builtin_return_address(4) : \
    (depth) == 5 ? __builtin_return_address(5) : \
    (depth) == 6 ? __builtin_return_address(6) : \
    (depth) == 7 ? __builtin_return_address(7) : \
    (depth) == 8 ? __builtin_return_address(8) : \
    (depth) == 9 ? __builtin_return_address(9) : \
    nullptr) \

# define CLEAN_APPEND(buff, value, size) \
     my_memset(buff,0,size); \
     my_strcat(buff,value);   \


/**
 * 目前最多支持10级调用栈
 */
#define GET_STACK_LEVEL 1


# define DL_INFO \
    std::list<TracerBase::stack_info> info;             \
    Dl_info dl_info = {0}; \
    Dl_info fast_info = {0}; \
    for(int i = 0; i<GET_STACK_LEVEL;i++){ \
        void* tag_ptr = GET_RETURN_ADDRESS(i);         \
        if(tag_ptr == nullptr){                 \
             continue;    \
        }          \
        int addr_ret_0 = dladdr(tag_ptr, &dl_info); \
        if(addr_ret_0 == 0){                 \
             continue;    \
        }        \
        if(i==0){\
            addr_ret_0 = dladdr(tag_ptr, &fast_info);                           \
        }      \
        TracerBase::stack_info tag_stack_info = {0};         \
        tag_stack_info.ptr = tag_ptr;         \
        tag_stack_info.stack_info.dli_fbase = dl_info.dli_fbase;         \
        tag_stack_info.stack_info.dli_saddr = dl_info.dli_saddr;                \
        if (dl_info.dli_fname) { \
            strncpy(tag_stack_info.stack_info.dli_fname, dl_info.dli_fname, sizeof(tag_stack_info.stack_info.dli_fname) - 1); \
            tag_stack_info.stack_info.dli_fname[sizeof(tag_stack_info.stack_info.dli_fname) - 1] = '\0'; \
        } \
        if (dl_info.dli_sname) { \
            strncpy(tag_stack_info.stack_info.dli_sname, dl_info.dli_sname, sizeof(tag_stack_info.stack_info.dli_sname) - 1); \
            tag_stack_info.stack_info.dli_sname[sizeof(tag_stack_info.stack_info.dli_sname) - 1] = '\0'; \
        } \
        info.push_back(tag_stack_info);         \
        \
    }             \



# define IS_MATCH \
        if(TracerBase::isLister(&fast_info)){ \

# define WRITE TracerBase::write \



class TracerBase : public ITracerBase {
public:
    typedef struct {
        char dli_fname[256];
        void* dli_fbase;
        char dli_sname[256];
        void* dli_saddr;
    } my_dl_info;

    struct stack_info {
        /**
         * 当前函数的调用地址
         */
        void *ptr;
        /**
         * 保存栈的调用信息
         */
        my_dl_info stack_info;
    };

    void init(JNIEnv *env,
                      bool hookAll,
                      const std::list<std::string> &forbid_list,
                      const std::list<std::string> &filter_list,
                      std::ofstream *os) override = 0;

    void stop() override = 0;


    bool isInited ;

    static bool isHookAll;
    static std::ofstream *traceOs;
    static std::list<std::string> filterSoList;
    static std::list<std::string> forbidSoList;
    static bool isSave;
    static std::string match_so_name;

    /**
      * 是否监听第二级返回结果。
    */
    bool isHookSecondRet;

    static void write(char *msg, std::list<TracerBase::stack_info> info);

    static void write(const char *msg, std::list<TracerBase::stack_info> info);

    static void write(const std::string &msg, std::list<TracerBase::stack_info> info);

    static void write(const std::wstring &msg, std::list<TracerBase::stack_info> info);

    static void write(const std::wstring *msg, std::list<TracerBase::stack_info> info);

    static void write(const std::string &msg);

    static void write(const std::string &msg, [[maybe_unused]] bool isApart);

    static void write(std::string &msg, std::list<TracerBase::stack_info> info);

    static char *getAddressHex(void *ptr);

    static bool isLister(Dl_info *info);

    static bool isAppFile(const char *path);

    static std::string getFileNameForPath(const char *path);


private:
    static void write_inline(const char *msg, std::list<TracerBase::stack_info> info);

    static void write_inline(const char *msg, std::list<TracerBase::stack_info> info, bool isApart);

};


#endif //RUNTIME_TRACERBASE_H
