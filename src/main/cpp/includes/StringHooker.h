//
// Created by zhenxi on 2022/1/21.
//
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

#include "mylibc.h"


/**
 * hook string append
 */
class StringHooker {

public:

    static void hookStrHandler(bool hookAll,
                               const std::list<std::string> &forbid_list,
                               const std::list<std::string> &filter_list,
                               std::ofstream *os);

    static void stopjnitrace();

private:
    static void init();

    /**
    * 是否监听第二级返回结果。
    */
    static bool isHookSecondRet ;
    static std::ofstream *hookStrHandlerOs;
    static char *filterSoList[100] ;
    static char *forbidSoList[100] ;
    static bool isHookAll ;
    static bool isSave ;
    static char match_so_name[MATCH_SO_NAME_SIZE] ;

    static bool isAppFile(const char *path) {
        if (my_strstr(path, "/data/") != nullptr) {
            return true;
        }
        return false;
    }

    static const char *getFileNameForPath(const char *path) {
        const char *fileName = strrchr(path, '/');
        if (fileName != nullptr) {
            return fileName + 1;
        }
        return path;
    }



    static bool isLister(int dladd_ret, Dl_info *info) {
        if (dladd_ret == 0) {
            return false;
        }
        if (info == nullptr) {
            return false;
        }
        const char *name = info->dli_fname;

        if (name == nullptr) {
            return false;
        }
        //系统apk暂不处理,只监听当前apk包下的
        if (!isAppFile(name)) {
            return false;
        }
        //如果是已经过滤的apk也暂不处理
        //比如我们注入的SO文件
        for (auto forbid: forbidSoList) {
            if (my_strstr(name, forbid) != nullptr) {
                //找到了则不进行处理
                return false;
            }
        }
        //如果是监听全部直接返回true
        if (isHookAll) {
            CLEAN_APPEND(match_so_name, getFileNameForPath(name), MATCH_SO_NAME_SIZE);
            return true;
        } else {
            //根据关键字进行过滤
            for (auto filter: filterSoList) {
                //默认监听一级
                if (my_strstr(name, filter) != nullptr) {
                    CLEAN_APPEND(match_so_name, getFileNameForPath(name), MATCH_SO_NAME_SIZE);
                    return true;
                }
            }
            return false;
        }
    }

    static char *getAddressHex(void *ptr) {
        if (ptr == nullptr) {
            return nullptr;
        }
        char *buffer = (char *) malloc(20);
        sprintf(buffer, "0x%lx", (uintptr_t) ptr);
        return buffer;
    }

    static void write(const char *msg, Dl_info info) {
        INIT_ORIG_BUFF
        if ((size_t) info.dli_fbase > 0) {
            GET_ADDRESS
            if (address != nullptr) {
                APPEND(buff, "<")
                APPEND(buff, address)
                APPEND(buff, ">")
                free((void *) address);
            }
        }
        APPEND(buff, "[")
        APPEND(buff, match_so_name)
        APPEND(buff, "]")
        APPEND(buff, msg)
        if (isSave) {
            if (hookStrHandlerOs != nullptr) {
                (*hookStrHandlerOs) << buff;
            }
        }
        LOG(INFO) << buff;
        if (msg != nullptr) {
            free((void *) msg);
        }
        if (buff != nullptr) {
            free((void *) buff);
        }
    }
};






