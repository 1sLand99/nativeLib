//
// Created by Zhenxi on 2024/7/8.
//

#include "TracerBase.h"

void TracerBase::write(const char *msg, Dl_info info) {
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

char *TracerBase::getAddressHex(void *ptr) {
    if (ptr == nullptr) {
        return nullptr;
    }
    char *buffer = (char *) malloc(20);
    sprintf(buffer, "0x%lx", (uintptr_t) ptr);
    return buffer;
}

bool TracerBase::isLister(int dladd_ret, Dl_info *info) {
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

const char *TracerBase::getFileNameForPath(const char *path) {
    const char *fileName = strrchr(path, '/');
    if (fileName != nullptr) {
        return fileName + 1;
    }
    return path;
}