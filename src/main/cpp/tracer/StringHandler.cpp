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

#include "StringHandler.h"
#include "TracerBase.h"

#include "HookUtils.h"
#include "mylibc.h"
#include "stringUtils.h"
#include "libpath.h"
#include "ZhenxiLog.h"
#include "common_macros.h"


using namespace StringUtils;


#define STRING_HANDLER_HOOK_DEF(ret, func, ...) \
  ret (*string_handler_orig_##func)(__VA_ARGS__)= nullptr; \
  ret string_handler_new_##func(__VA_ARGS__)

#define HOOK_SYMBOL_DOBBY(handle, func)  \
  hook_libc_function(handle, #func, (void*) string_handler_new_##func, (void**) &string_handler_orig_##func); \


/**
 * 如果需要hook一些string处理的函数,所以尽可能使用C语言去实现这些功能。
 * 否则会和string hook冲突
 */
// char* strstr(char* h, const char* n)
STRING_HANDLER_HOOK_DEF(char*, strstr, char *h, const char *n) {
    DL_INFO
    IS_MATCH char *ret = string_handler_orig_strstr(h, n);
        if (ret != nullptr) {
            INIT_ORIG_BUFF
            APPEND(buff, "strstr() arg1 -> ")
            APPEND(buff, IS_NULL(h))
            APPEND(buff, "  arg2-> ")
            APPEND(buff, IS_NULL(n))
            APPEND(buff, "\n")
            WRITE(buff, info);
        }
        return ret;

    }
    return string_handler_orig_strstr(h, n);
}

//size_t strlen(const char* __s)
STRING_HANDLER_HOOK_DEF(size_t, strlen, const char *__s) {
    DL_INFO
    IS_MATCH size_t ret = string_handler_orig_strlen(__s);
        if (ret > 0) {
            INIT_ORIG_BUFF
            APPEND(buff, "strlen() arg1 -> ")
            APPEND(buff, IS_NULL(__s))
            APPEND(buff, "\n")
            WRITE(buff, info);
        }
        return ret; }
    return string_handler_orig_strlen(__s);
}
//size_t __strlen_chk(const char *a1, size_t a2)
STRING_HANDLER_HOOK_DEF(size_t, __strlen_chk, const char *__s, size_t a2) {
    DL_INFO
    IS_MATCH
        size_t ret = string_handler_orig___strlen_chk(__s, a2);
        if (ret > 0) {
            INIT_ORIG_BUFF
            APPEND(buff, "strlen_chk() arg1 -> ")
            APPEND(buff, IS_NULL(__s))
            APPEND(buff, "\n")
            WRITE(buff, info);
        }
        return ret;
    }
    return string_handler_orig___strlen_chk(__s, a2);
}
//char* strcat(char* __dst, const char* __src);
STRING_HANDLER_HOOK_DEF(char *, strcat, char *__dst, const char *__src) {
    DL_INFO
    IS_MATCH char *ret = string_handler_orig_strcat(__dst, __src);
        INIT_ORIG_BUFF
        APPEND(buff, "strcat() arg1 -> ")
        APPEND(buff, IS_NULL(__dst))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL(__src))
        if (ret != nullptr) {
            APPEND(buff, " result -> ")
            APPEND(buff, IS_NULL(ret))
        }
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }
    return string_handler_orig_strcat(__dst, __src);
}
//int strcmp(const char* __lhs, const char* __rhs)
STRING_HANDLER_HOOK_DEF(int, strcmp, const char *__lhs, const char *__rhs) {
    DL_INFO
    IS_MATCH int ret = string_handler_orig_strcmp(__lhs, __rhs);
        INIT_ORIG_BUFF
        APPEND(buff, "strcmp() arg1 -> ")
        APPEND(buff, IS_NULL(__lhs))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL(__rhs))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret; }
    return string_handler_orig_strcmp(__lhs, __rhs);
}
//char* strcpy(char* __dst, const char* __src);
STRING_HANDLER_HOOK_DEF(char *, strcpy, char *__dst, const char *__src) {

    DL_INFO
    IS_MATCH char *ret = string_handler_orig_strcpy(__dst, __src);
        INIT_ORIG_BUFF
        APPEND(buff, "strcpy() arg1 -> ")
        APPEND(buff, IS_NULL(__dst))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL(__src))

        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }
    return string_handler_orig_strcpy(__dst, __src);
}
//int sprintf(char* __s, const char* __fmt, ...)
STRING_HANDLER_HOOK_DEF(int, sprintf, char *__s, const char *__fmt, char *p...) {
    DL_INFO
    IS_MATCH int ret = string_handler_orig_sprintf(__s, __fmt, p);
        INIT_ORIG_BUFF
        APPEND(buff, "sprintf() arg1 -> ")
        APPEND(buff, IS_NULL(__s))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL(__fmt))
        APPEND(buff, "  arg3-> ")
        APPEND(buff, IS_NULL(p))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }

    return string_handler_orig_sprintf(__s, __fmt, p);
}
//int printf(const char* __fmt, ...)
STRING_HANDLER_HOOK_DEF(int, printf, const char *__fmt, char *p...) {
    DL_INFO
    IS_MATCH int ret = string_handler_orig_printf(__fmt, p);
        INIT_ORIG_BUFF
        APPEND(buff, "printf() arg1 -> ")
        APPEND(buff, IS_NULL(__fmt))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL(p))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }

    return string_handler_orig_printf(__fmt, p);
}
//char *strtok(char *str, const char *delim)
STRING_HANDLER_HOOK_DEF(char *, strtok, char *str, const char *delim) {
    DL_INFO
    IS_MATCH char *ret = string_handler_orig_strtok(str, delim);
        INIT_ORIG_BUFF
        APPEND(buff, "strtok() arg1 -> ")
        APPEND(buff, IS_NULL(str))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL(delim))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret; }
    return string_handler_orig_strtok(str, delim);
}
//char* strdup(const char* __s);
STRING_HANDLER_HOOK_DEF(char*, strdup, char *__s) {

    DL_INFO
    IS_MATCH
        char *ret = string_handler_orig_strdup(__s);
        INIT_ORIG_BUFF
        APPEND(buff, "strdup() arg1 -> ")
        APPEND(buff, IS_NULL(__s))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }
    return string_handler_orig_strdup(__s);
}


//把 str1 和 str2 进行比较，结果取决于 LC_COLLATE 的位置设置。
//int strcoll(const char* __lhs, const char* __rhs) __attribute_pure__;
STRING_HANDLER_HOOK_DEF(int, strcoll, const char *__lhs, const char *__rhs) {
    DL_INFO
    IS_MATCH int ret = string_handler_orig_strcoll(__lhs, __rhs);

        INIT_ORIG_BUFF
        APPEND(buff, "strcoll() arg1 -> ")
        APPEND(buff, IS_NULL(__lhs))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL(__rhs))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }
    return string_handler_orig_strcoll(__lhs, __rhs);
}
//size_t strxfrm(char *dest, const char *src, size_t n)
STRING_HANDLER_HOOK_DEF(size_t, strxfrm, char *dest, const char *src, size_t n) {

    DL_INFO
    IS_MATCH size_t ret = string_handler_orig_strxfrm(dest, src, n);
        INIT_ORIG_BUFF
        APPEND(buff, "strxfrm() arg1 -> ")
        APPEND(buff, IS_NULL(dest))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL(src))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }
    return string_handler_orig_strxfrm(dest, src, n);
}
//char* fgets(char* __buf, int __size, FILE* __fp);
STRING_HANDLER_HOOK_DEF(char*, fgets, char *__buf, int __size, FILE *__fp) {
    DL_INFO
    IS_MATCH char *ret = string_handler_orig_fgets(__buf, __size, __fp);
        INIT_ORIG_BUFF
        APPEND(buff, "fgets() arg1 -> ")
        APPEND(buff, IS_NULL(__buf))
        APPEND(buff, "  arg2-> ")
        APPEND_INT(buff, __size)
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }
    return string_handler_orig_fgets(__buf, __size, __fp);
}
//void *memcpy(void *destin, void *source, unsigned n);
STRING_HANDLER_HOOK_DEF(void*, memcpy, void *destin, void *source, size_t __n) {
    DL_INFO
    IS_MATCH

        void *ret;
        if (string_handler_orig_memcpy == nullptr) {
            ret = string_handler_orig_memcpy(destin, source, __n);
        } else {
            ret = my_memcpy(destin, source, __n);
        }
        if (source == nullptr || destin == nullptr) {
            return ret;
        }
        INIT_ORIG_BUFF
        APPEND(buff, "memcpy() arg1 -> ")
        APPEND(buff, IS_NULL((char *) destin))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL((char *) source))
        APPEND(buff, "  arg3-> ")
        APPEND_INT(buff, __n)
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;
    }

    return string_handler_orig_memcpy(destin, source, __n);
}
//int snprintf(char* __buf, size_t __size, const char* __fmt, ...) __printflike(3, 4);
STRING_HANDLER_HOOK_DEF(int, snprintf, char *__buf, size_t __size, const char *__fmt, char *p  ...) {
    DL_INFO
    IS_MATCH int ret = string_handler_orig_snprintf(__buf, __size, __fmt, p);
        if (ret == -1) {
            return ret;
        }
        INIT_ORIG_BUFF
        APPEND(buff, "snprintf() arg1 -> ")
        APPEND(buff, IS_NULL(__buf))
        APPEND(buff, "  arg2-> ")
        APPEND_INT(buff, __size)
        APPEND(buff, "  arg3-> ")
        APPEND(buff, IS_NULL(__fmt))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }

    return string_handler_orig_snprintf(__buf, __size, __fmt, p);
}

STRING_HANDLER_HOOK_DEF(void *, malloc, size_t __byte_count) {
    DL_INFO
    IS_MATCH void *ret = string_handler_orig_malloc(__byte_count);
        if (ret == nullptr || __byte_count == 0) {
            return ret;
        }
        INIT_ORIG_BUFF
        APPEND(buff, "malloc() arg1 -> ")
        APPEND_INT(buff, __byte_count)
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }
    return string_handler_orig_malloc(__byte_count);
}
//int vsnprintf(char* __buf, size_t __size, const char* __fmt, va_list __args) __printflike(3, 0);
STRING_HANDLER_HOOK_DEF(int, vsnprintf, char *__buf, size_t __size, const char *__fmt, va_list __args) {
    DL_INFO
    IS_MATCH
        int ret = string_handler_orig_vsnprintf(__buf, __size, __fmt, __args);
        INIT_ORIG_BUFF
        APPEND(buff, "vsnprintf() arg1 -> ")
        APPEND(buff, IS_NULL(__buf))
        APPEND(buff, "  arg2-> ")
        APPEND_INT(buff, __size)
        APPEND(buff, "  arg3-> ")
        APPEND(buff, IS_NULL(__fmt))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;
    }
    return string_handler_orig_vsnprintf(__buf, __size, __fmt, __args);
}
//void *memmove(void *dest, const void *src, size_t n);
STRING_HANDLER_HOOK_DEF(void *, memmove, void *dest, const void *src, size_t n) {
    DL_INFO
    IS_MATCH auto ret = string_handler_orig_memmove(dest, src, n);
        INIT_ORIG_BUFF
        APPEND(buff, "vsnprintf() arg1 -> ")
        APPEND(buff, IS_NULL((char *) dest))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL((char *) src))
        APPEND(buff, "  arg3-> ")
        APPEND_INT(buff, n)
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;

    }

    return string_handler_orig_memmove(dest, src, n);
}
//char *strncat(char *dest, const char *src, size_t n)
STRING_HANDLER_HOOK_DEF(char *, strncat, char *dest, const char *src, size_t n) {
    DL_INFO
    IS_MATCH
        auto ret = string_handler_orig_strncat(dest, src, n);
        INIT_ORIG_BUFF
        APPEND(buff, "strncat() arg1 -> ")
        APPEND(buff, IS_NULL((char *) dest))
        APPEND(buff, "  arg2-> ")
        APPEND(buff, IS_NULL((char *) src))
        APPEND(buff, "  arg3-> ")
        APPEND_INT(buff, n)
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;
    }
    return string_handler_orig_strncat(dest, src, n);
}
//char *strlwr(char *str)
STRING_HANDLER_HOOK_DEF(char *, strlwr, char *str) {
    DL_INFO
    IS_MATCH
        auto ret = string_handler_orig_strlwr(str);
        INIT_ORIG_BUFF
        APPEND(buff, "strlwr() arg1 -> ")
        APPEND(buff, IS_NULL((char *) str))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;
    }
    return string_handler_orig_strlwr(str);
}
//char *strupr(char *str)
STRING_HANDLER_HOOK_DEF(char *, strupr, char *str) {
    DL_INFO
    IS_MATCH
        auto ret = string_handler_orig_strupr(str);
        INIT_ORIG_BUFF
        APPEND(buff, "strupr() arg1 -> ")
        APPEND(buff, IS_NULL((char *) str))
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;
    }
    return string_handler_orig_strupr(str);
}
//char *strchr(const char *str, int c)
STRING_HANDLER_HOOK_DEF(char *, strchr, const char *str, int c) {
    DL_INFO
    IS_MATCH
        auto ret = string_handler_orig_strchr(str, c);
        INIT_ORIG_BUFF
        APPEND(buff, "strchr() arg1 -> ")
        APPEND(buff, IS_NULL((char *) str))
        APPEND(buff, "  arg2-> ")
        APPEND_INT(buff, c)
        APPEND(buff, "\n")
        WRITE(buff, info);
        return ret;
    }
    return string_handler_orig_strchr(str, c);
}


void init_string_handler() {
    void *handle = dlopen("libc.so", RTLD_NOW);

    if (handle == nullptr) {
        LOG(ERROR) << "str handler get handle == null   ";
        return;
    }

    HOOK_SYMBOL_DOBBY(handle, strlen)
    //strlen buff check
    HOOK_SYMBOL_DOBBY(handle, __strlen_chk)
    HOOK_SYMBOL_DOBBY(handle, strcmp)
    HOOK_SYMBOL_DOBBY(handle, strstr)
    HOOK_SYMBOL_DOBBY(handle, fgets)
    HOOK_SYMBOL_DOBBY(handle, strcpy)
    HOOK_SYMBOL_DOBBY(handle, strdup)

//    HOOK_SYMBOL_DOBBY(handle, strxfrm)
//    HOOK_SYMBOL_DOBBY(handle, strtok)
//    HOOK_SYMBOL_DOBBY(handle, strncat)
//    HOOK_SYMBOL_DOBBY(handle, strlwr)
//    HOOK_SYMBOL_DOBBY(handle, strupr)
//    HOOK_SYMBOL_DOBBY(handle, strchr)

    //strcat底层走的是strcpy
    //https://cs.android.com/android/platform/superproject/+/master:external/musl/src/string/strcat.c;l=5?q=strcat
//    HOOK_SYMBOL_DOBBY(handle, strcat)
    //strcoll底层是strcmp
    //https://cs.android.com/android/platform/superproject/+/master:external/musl/src/locale/strcoll.c;l=12;drc=master?q=strcoll&ss=android%2Fplatform%2Fsuperproject
//    HOOK_SYMBOL_DOBBY(handle, strcoll)


    //HOOK_SYMBOL_DOBBY(handle, sprintf)
    //HOOK_SYMBOL_DOBBY(handle, printf)
    //HOOK_SYMBOL_DOBBY(handle, snprintf)
    //HOOK_SYMBOL_DOBBY(handle, vsnprintf)

//    HOOK_SYMBOL_DOBBY(handle, memmove)
//    HOOK_SYMBOL_DOBBY(handle, memcpy)




    dlclose(handle);

    LOG(ERROR) << ">>>>>>>>> string handler init success !  ";
}

void StringHandler::init(JNIEnv *env,
                         bool hookAll,
                         const std::list<std::string> &forbid_list,
                         const std::list<std::string> &filter_list,
                         std::ofstream *os) {
    if(isInited){
        return;
    }
    isInited = true;
    isHookAll = hookAll;
    LOGE("start StringHandler trace is hook all %s", isHookAll ? "true" : "false")
    for (const std::string &str: forbid_list) {
        LOGE("start StringHandler trace forbid_list %s", str.c_str())
    }

    //copy orig list
    forbidSoList = std::list<string>(forbid_list);
    filterSoList = std::list<string>(filter_list);
    if (os != nullptr) {
        isSave = true;
        traceOs = os;
    }

    init_string_handler();
}

[[maybe_unused]]
void StringHandler::stop() {
    filterSoList.clear();
    forbidSoList.clear();
    if (traceOs != nullptr) {
        if (traceOs->is_open()) {
            traceOs->close();
        }
        delete traceOs;
    }
    isSave = false;
}

StringHandler::StringHandler() {

}


