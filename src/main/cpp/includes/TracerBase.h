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

#define  ORIG_BUFF_SIZE  100

#define  MATCH_SO_NAME_SIZE  100

#define  MAX_PRINTF_SIZE  PATH_MAX

#define INIT_ORIG_BUFF \
    char *buff = (char*)malloc(ORIG_BUFF_SIZE); \
    my_memset(buff,0,ORIG_BUFF_SIZE); \

#define HOOK_SYMBOL_DOBBY(handle, func)  \
  hook_libc_function(handle, #func, (void*) new_##func, (void**) &orig_##func); \

//这块是为了防止他把一个void*当成char*去strlen挂掉
# define IS_NULL(value) value \

# define APPEND_INT(buff, value) \
     char* int_value = int_to_str(value);           \
     if(int_value!=nullptr){                           \
        APPEND(buff,int_value);                   \
        free(int_value);                           \
     }

# define APPEND(buff, value) \
     if(my_strlen(value)>0){     \
        buff = (char *)realloc(buff,my_strlen(buff)+my_strlen(value)+1); \
        my_strcat(buff,value);  \
     }  \

#define GET_ADDRESS \
 auto address = getAddressHex((void*)((char *) \
            __builtin_return_address(0) - ((size_t) info.dli_fbase))); \


# define CLEAN_APPEND(buff, value, size) \
     my_memset(buff,0,size); \
     my_strcat(buff,value);   \

# define DL_INFO \
    Dl_info info={0}; \
    int addr_ret_0 = dladdr((void *) __builtin_return_address(0), &info); \

# define IS_MATCH \
        if(StringHooker::isLister(addr_ret_0,&info)){  \


class TracerBase {
    public:
        static void write(const char *msg, Dl_info info) ;
        static char *getAddressHex(void *ptr);
        static bool isLister(int dladd_ret, Dl_info *info);
        static bool isAppFile(const char *path);
        static const char *getFileNameForPath(const char *path);
};


#endif //RUNTIME_TRACERBASE_H
