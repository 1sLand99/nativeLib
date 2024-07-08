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

#include "StringHooker.h"

void StringHooker::hookStrHandler(bool hookAll, const std::list<string> &forbid_list,
                                   const std::list<string> &filter_list, std::ofstream *os) {
    isHookAll = hookAll;

    int i = 0;
    for (const auto &string: forbid_list) {
        forbidSoList[i] = strdup(string.c_str());
        LOG(ERROR) << "forbidSoList -> " << i << " " << forbidSoList[i];
        i++;
    }
    i = 0;
    for (const auto &string: filter_list) {
        filterSoList[i] = strdup(string.c_str());
        LOG(ERROR) << "filterSoList -> " << i << " " << filterSoList[i];
        i++;
    }
    if (os != nullptr) {
        isSave = true;
        hookStrHandlerOs = os;
    } else {
        LOG(ERROR) << ">>>>>>>>>>>>> string handler init fail hookStrHandlerOs == null  ";
    }
    init();
}

[[maybe_unused]]
void StringHooker::stopjnitrace() {
    for (auto str: forbidSoList) {
        free(str);
    }
    for (auto str: filterSoList) {
        free(str);
    }
    if (hookStrHandlerOs != nullptr) {
        if (hookStrHandlerOs->is_open()) {
            hookStrHandlerOs->close();
        }
        delete hookStrHandlerOs;
    }
    isSave = false;
}

const char *getFileNameForPath(const char *path) {
    const char *fileName = strrchr(path, '/');
    if (fileName != nullptr) {
        return fileName + 1;
    }
    return path;
}




