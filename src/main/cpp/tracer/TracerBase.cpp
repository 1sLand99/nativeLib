//
// Created by Zhenxi on 2024/7/8.
//
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
#include <utility>

#include "TracerBase.h"
#include "mylibc.h"

using namespace std;


bool TracerBase::isHookAll = false;
std::ofstream *TracerBase::traceOs = {};
std::list<string> TracerBase::filterSoList = {};
std::list<string> TracerBase::forbidSoList = {};
bool TracerBase::isSave = false;






void TracerBase::write_inline(const char *msg, std::list<TracerBase::stack_info> info) {
    write_inline(msg, std::move(info), false);
}

void TracerBase::write_inline(const char *msg,
                              std::list<TracerBase::stack_info> info,
                              bool isApart) {
    if (msg == nullptr || my_strlen(msg) == 0) {
        return;
    }
    //内容输出 ...
    //[所属ELF]{所属函数地址名称}<具体函数地址>
    //[所属ELF]{所属函数地址名称}<具体函数地址>
    //[所属ELF]{所属函数地址名称}<具体函数地址>
    //[所属ELF]{所属函数地址名称}<具体函数地址>
    //...
    //----------------------------------------------
    string buff = {};
    buff.append(msg).append("\n");
    //如果是分割线则不需要保存具体的地址
    if (!isApart) {
        for (auto & it : info) {
            buff.append("[");
            buff.append(getFileNameForPath(it.stack_info.dli_fname));
            buff.append("]");
            const char *sname = it.stack_info.dli_sname;
            if(my_strlen(sname)>0){
                buff.append("{");
                buff.append(sname);
                buff.append("}");
            }
            if (it.ptr != nullptr) {
                buff.append("<");
                buff.append(getAddressHex((char*)it.ptr-(size_t)it.stack_info.dli_fbase));
                buff.append(">");
            }
            if(info.size()>1){
                buff.append("\n");
            }
        }
        if(info.size()>1) {
            buff.append("-----------------------------------").append("\n");
        }
    }
    if (isSave) {
        if (traceOs != nullptr) {
            (*traceOs) << buff;
        }
    }
    LOG(INFO) <<buff;
}

/**
 * 第二个参数标识当前是否是分隔符
 * 如果是分隔符是没有调用栈的。
 */
void TracerBase::write(const std::string &msg, [[maybe_unused]] bool isApart) {
    std::list<TracerBase::stack_info> info = {};
    write_inline(msg.c_str(), info, isApart);
}

void TracerBase::write(const std::string &msg) {
    std::list<TracerBase::stack_info> info = {};
    write_inline(msg.c_str(), info);
}

void TracerBase::write(const std::wstring &msg, std::list<TracerBase::stack_info> info) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    auto ret = converter.to_bytes(msg);
    write_inline(ret.c_str(), std::move(info));
}

void TracerBase::write(const std::wstring *msg, std::list<TracerBase::stack_info> info) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    auto ret = converter.to_bytes(*msg);
    write_inline(ret.c_str(), std::move(info));
}

void TracerBase::write(std::string &msg, std::list<TracerBase::stack_info> info) {
    write_inline(msg.c_str(), std::move(info));
}


void TracerBase::write(const std::string &msg, std::list<TracerBase::stack_info> info) {
    write_inline(msg.c_str(), std::move(info));
}

void TracerBase::write(char *msg, std::list<TracerBase::stack_info> info) {
    write_inline(msg, std::move(info));
}

void TracerBase::write(const char *msg, std::list<TracerBase::stack_info> info) {
    write_inline(msg, std::move(info));
}

char *TracerBase::getAddressHex(void *ptr) {
    static char buffer[20];
    if (ptr == nullptr) {
        return nullptr;
    }
    auto ret = (uintptr_t) ptr;
    if (ret<=0){
        return nullptr;
    }
    snprintf(buffer, sizeof(buffer), "0x%lx", ret);
    return buffer;
}


bool TracerBase::isLister(Dl_info *info) {
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
    for (const auto &forbid: forbidSoList) {
        if (my_strstr(name, forbid.c_str()) != nullptr) {
            //找到了则不进行处理
            return false;
        }
    }
    //如果是监听全部直接返回true
    if (isHookAll) {
        return true;
    } else {
        //根据关键字进行过滤
        for (const auto &filter: filterSoList) {
            //默认监听一级
            if (my_strstr(name, filter.c_str()) != nullptr) {
                return true;
            }
        }
        return false;
    }
}

inline string TracerBase::getFileNameForPath(const char *path) {
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