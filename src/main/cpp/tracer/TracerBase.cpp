//
// Created by Zhenxi on 2024/7/8.
//

#include "TracerBase.h"
#include "mylibc.h"

using namespace std;


bool TracerBase::isHookAll = false;
std::ofstream *TracerBase::traceOs = {};
std::list<string> TracerBase::filterSoList = {};
std::list<string> TracerBase::forbidSoList = {};
bool TracerBase::isSave = false;
string TracerBase::match_so_name = {};
/**
 * 是否监听第二级返回结果。
*/
bool TracerBase::isHookSecondRet;

//jmethodID TracerBase::object_method_id_toString = nullptr;
//jclass TracerBase:: AuxiliaryClazz = nullptr;
//jmethodID TracerBase:: AuxiliaryClazz_method_id_toString = nullptr;


/**
 * 第二个参数标识当前是否是分隔符
 */
void TracerBase::write(const std::string &msg, [[maybe_unused]] bool isApart) {
    //std::unique_lock<std::mutex> mock(supernode_ids_mux_);
    if (msg.c_str() == nullptr || msg.empty()) {
        return;
    }
    if (isSave) {
        if (traceOs != nullptr) {
            (*traceOs) << msg.c_str();
        }
    }
    if (isApart) {
        LOG(INFO) << msg.c_str();
    } else {
        LOG(INFO) << "[" << match_so_name << "] " << msg.c_str();
    }

}
void TracerBase::write(const std::string &msg) {
    //写入方法加锁,防止多进程导致问题
    //std::unique_lock<std::mutex> mock(supernode_ids_mux_);
    if (msg.c_str() == nullptr || msg.empty()) {
        return;
    }
    if (isSave) {
        if (traceOs != nullptr) {
            (*traceOs) << "[" << match_so_name << "]" << msg.c_str();
        }
    }
    LOG(INFO) << "[" << match_so_name << "] " << msg.c_str();
}

void TracerBase::write(const std::string &msg, Dl_info info) {
    string buff = {};
    if ((size_t) info.dli_fbase > 0) {
        GET_ADDRESS
        if (address != nullptr) {
            buff.append("<");
            buff.append(address);
            buff.append(">");
        }
    }
    buff.append("[");
    buff.append(match_so_name);
    buff.append("]");
    buff.append(msg);

    if (isSave) {
        if (traceOs != nullptr) {
            (*traceOs) << buff;
        }
    }
    LOG(INFO) << buff;

}
void TracerBase::write(const char *msg, Dl_info info) {
    string buff = {};
    if ((size_t) info.dli_fbase > 0) {
        GET_ADDRESS
        if (address != nullptr) {
            buff.append("<");
            buff.append(address);
            buff.append(">");
        }
    }
    buff.append("[");
    buff.append(match_so_name);
    buff.append("]");
    buff.append(msg);

    if (isSave) {
        if (traceOs != nullptr) {
            (*traceOs) << buff;
        }
    }
    LOG(INFO) << buff;

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
    for (const auto& forbid: forbidSoList) {
        if (my_strstr(name, forbid.c_str()) != nullptr) {
            //找到了则不进行处理
            return false;
        }
    }
    //如果是监听全部直接返回true
    if (isHookAll) {
        match_so_name = {};
        return true;
    } else {
        //根据关键字进行过滤
        for (const auto& filter: filterSoList) {
            //默认监听一级
            if (my_strstr(name, filter.c_str()) != nullptr) {
                match_so_name = getFileNameForPath(name);
                return true;
            }
        }
        return false;
    }
}

string TracerBase::getFileNameForPath(const char *path) {
    if (path == nullptr) {
        return {};
    }
    std::string pathStr{path};
    size_t pos = pathStr.rfind('/');
    if (pos != std::string::npos) {
        return pathStr.substr(pos + 1);
    }
    return pathStr;
}



bool TracerBase::isAppFile(const char *path) {
    if (my_strstr(path, "/data/") != nullptr) {
        return true;
    }
    return false;
}