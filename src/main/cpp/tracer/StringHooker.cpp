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
#include "ZhenxiLog.h"
#include "linkerHandler.h"
#include "xdl.h"
#include "HookUtils.h"

using namespace std;


#define STRING_HOOKER_HOOK_DEF(ret, func, ...) \
  ret (*string_hooker_orig_##func)(__VA_ARGS__) = nullptr; \
  ret string_hooker_new_##func(__VA_ARGS__)



#define StringHookerSym(sym_str, hooker) \
        sym = xdl_sym(handler, sym_str, nullptr); \
        if(sym == nullptr){    \
            sym = xdl_dsym(handler, sym_str, nullptr); \
        }\
        if(sym != nullptr) {             \
           ret = HookUtils::Hooker(sym,(void*)string_hooker_new_##hooker,(void**)&string_hooker_orig_##hooker); \
            if(!ret){                       \
                LOGE("StringHookerSym hook error %s", sym_str)                                \
            }                                  \
        } else{                          \
             LOGE("StringHookerSym find error %s", sym_str)                             \
        }                               \
        sym = nullptr;                   \




//---------------cunstruction---------------------------------------
/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_mmRKS4_
 * 2、std::string::basic_string(const std::string& str, size_t pos, size_t n, const std::allocator<char>& alloc)
 */
STRING_HOOKER_HOOK_DEF(std::string &, construction_1, const std::string &str, size_t pos, size_t n, const std::allocator<char> &alloc) {
    std::string &ret = string_hooker_orig_construction_1(str, pos, n, alloc);
    DL_INFO
    WRITE(ret, info);
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_RKS4_
 * 2、std::string::basic_string(const std::string& str, const std::allocator<char>& alloc)
 */
STRING_HOOKER_HOOK_DEF(std::string &, construction_2, const std::string &str, const std::allocator<char> &alloc) {
    std::string &ret = string_hooker_orig_construction_2(str, alloc);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_
 * 2、std::string::basic_string(const std::string& str)
 */
STRING_HOOKER_HOOK_DEF(std::string &, construction_3, const std::string &str) {
    std::string &ret = string_hooker_orig_construction_3(str);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

//---------------append---------------------------------------------
/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendERKS5_mm
 * 2、std::string::append(const std::string& str, size_t pos, size_t n)
 */
STRING_HOOKER_HOOK_DEF(std::string &, append_1, const std::string &str, size_t pos, size_t n) {
    std::string &ret = string_hooker_orig_append_1(str, pos, n);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}
/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE23__append_forward_unsafeINS_11__wrap_iterIPcEEEERS5_T_SB_
 * 2、std::string::__append_forward_unsafe<std::__ndk1::__wrap_iter<char*> >(std::__ndk1::__wrap_iter<char*>, std::__ndk1::__wrap_iter<char*>)
 */
STRING_HOOKER_HOOK_DEF(std::string &, append_2, std::__ndk1::__wrap_iter<char *> iter1,
                       std::__ndk1::__wrap_iter<char *> iter2) {
    std::string &ret = string_hooker_orig_append_2(iter1, iter2);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEmc
 * 2、std::string::append(size_t n, char c)
 */
STRING_HOOKER_HOOK_DEF(std::string &, append_3, size_t n, char c) {
    std::string &ret = string_hooker_orig_append_3(n, c);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}
/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc
 * 2、std::string::append(const char* s)
 */
STRING_HOOKER_HOOK_DEF(std::string &, append_4, const char *s) {
    std::string &ret = string_hooker_orig_append_4(s);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE23__append_forward_unsafeIPcEERS5_T_S9_
 * 2、std::string::__append_forward_unsafe<char*>(char*, char*)
 */
STRING_HOOKER_HOOK_DEF(std::string &, append_5, char *iter1, char *iter2) {
    std::string &ret = string_hooker_orig_append_5(iter1, iter2);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm
 * 2、std::string::__append_forward_unsafe<char*>(char*, char*)
 */
STRING_HOOKER_HOOK_DEF(std::string &, append_6, const char *s, size_t n) {
    std::string &ret = string_hooker_orig_append_6(s, n);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}


/**
 * 1、_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6appendEmw
 * 2、std::wstring::append(size_t n, wchar_t c)
 */
STRING_HOOKER_HOOK_DEF(std::wstring &, wappend_1, size_t n, wchar_t c) {
    std::wstring &ret = string_hooker_orig_wappend_1(n, c);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}
/**
 * 1、_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE23__append_forward_unsafeIPwEERS5_T_S9_
 * 2、std::wstring::__append_forward_unsafe<wchar_t*>(wchar_t*, wchar_t*)
 */
STRING_HOOKER_HOOK_DEF(std::wstring &, wappend_2, wchar_t *iter1, wchar_t *iter2) {
    std::wstring &ret = string_hooker_orig_wappend_2(iter1, iter2);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6appendEPKwm
 * 2、std::wstring::append(const wchar_t* s, size_t n)
 */
STRING_HOOKER_HOOK_DEF(std::wstring &, wappend_3, const wchar_t *s, size_t n) {
    std::wstring &ret = string_hooker_orig_wappend_3(s, n);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6appendEPKw
 * 2、std::wstring::append(const wchar_t* s)
 */
STRING_HOOKER_HOOK_DEF(std::wstring &, wappend_4, const wchar_t *s) {
    std::wstring &ret = string_hooker_orig_wappend_4(s);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6appendERKS5_mm
 * 2、std::wstring::append(const std::wstring& str, size_t pos, size_t n)
 */
STRING_HOOKER_HOOK_DEF(std::wstring &, wappend_5, const std::wstring &str, size_t pos, size_t n) {
    std::wstring &ret = string_hooker_orig_wappend_5(str, pos, n);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignERKS5_mm
 * 2、std::string::assign(const std::string& str, size_t pos, size_t n)
 */
STRING_HOOKER_HOOK_DEF(std::string &, assign_1, const std::string &str, size_t pos, size_t n) {
    std::string &ret = string_hooker_orig_assign_1(str, pos, n);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEmc
 * 2、std::string::assign(size_t n, char c)
 */
STRING_HOOKER_HOOK_DEF(std::string &, assign_2, size_t n, char c) {
    std::string &ret = string_hooker_orig_assign_2(n, c);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc
 * 2、std::string::assign(const char* s)
 */
STRING_HOOKER_HOOK_DEF(std::string &, assign_3, const char *s) {
    std::string &ret = string_hooker_orig_assign_3(s);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKcm
 * 2、std::string::assign(const char* s, size_t n)
 */
STRING_HOOKER_HOOK_DEF(std::string &, assign_4, const char *s, size_t n) {
    std::string &ret = string_hooker_orig_assign_4(s, n);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}
/**
 * 1、_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6resizeEmw
 * 2、void std::basic_string<wchar_t>::resize(size_t n, wchar_t c)
 */
STRING_HOOKER_HOOK_DEF(void, resize_wchar, std::wstring *this_ptr, size_t n, wchar_t c) {
    string_hooker_orig_resize_wchar(this_ptr, n, c);
    DL_INFO
    IS_MATCH
        WRITE(*this_ptr, info);
    }
}
/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6resizeEmc
 * 2、void std::basic_string<char>::resize(size_t n, char c)
 */
STRING_HOOKER_HOOK_DEF(void, resize_char, std::string *this_ptr, size_t n, char c) {
    string_hooker_orig_resize_char(this_ptr, n, c);
    DL_INFO
    IS_MATCH
        WRITE(*this_ptr, info);
    }
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC2IDnEEPKc
 * 2、std::basic_string<char>::basic_string(const char*)
 */
STRING_HOOKER_HOOK_DEF(void, construction_4, std::string *this_ptr, const char *s) {
    string_hooker_orig_construction_4(this_ptr, s);
    DL_INFO
    IS_MATCH
        WRITE(*this_ptr, info);
    }
}
//----------------析构----------------------------
/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED2Ev
 * 2、std::basic_string<char>::~basic_string()
 */
STRING_HOOKER_HOOK_DEF(void, basic_string_dtor, std::string *this_ptr) {
    string_hooker_orig_basic_string_dtor(this_ptr);
    DL_INFO
    IS_MATCH
        WRITE(*this_ptr, info);
    }
}
//-------------------replace-------------------------
/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7replaceEmmPKcm
 * 2、std::string::replace(size_t pos, size_t len, const char* s, size_t n)
 */
STRING_HOOKER_HOOK_DEF(std::string&, replace_1, std::string *this_ptr, size_t pos, size_t len, const char *s, size_t n) {
    std::string &ret = string_hooker_orig_replace_1(this_ptr, pos, len, s, n);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7replaceEmmRKS5_mm
 * 2、std::string::replace(size_t pos1, size_t len1, const std::string& str, size_t pos2, size_t len2)
 */
STRING_HOOKER_HOOK_DEF(std::string&, replace_2, std::string *this_ptr, size_t pos1, size_t len1, const std::string& str, size_t pos2, size_t len2) {
    std::string &ret = string_hooker_orig_replace_2(this_ptr, pos1, len1, str, pos2, len2);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7replaceEmmmc
 * 2、std::string::replace(size_t pos, size_t len, size_t n, char c)
 */
STRING_HOOKER_HOOK_DEF(std::string&, replace_3, std::string *this_ptr, size_t pos, size_t len, size_t n, char c) {
    std::string &ret = string_hooker_orig_replace_3(this_ptr, pos, len, n, c);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7replaceEmmPKc
 * 2、std::string::replace(size_t pos, size_t len, const char* s)
 */
STRING_HOOKER_HOOK_DEF(std::string&, replace_4, std::string *this_ptr, size_t pos, size_t len, const char* s) {
    std::string &ret = string_hooker_orig_replace_4(this_ptr, pos, len, s);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}
//-----------------------insert----------------------------
/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmmc
 * 2、std::string::insert(size_t pos, size_t n, char c)
 */
STRING_HOOKER_HOOK_DEF(std::string&, insert_1, std::string *this_ptr, size_t pos, size_t n, char c) {
    std::string &ret = string_hooker_orig_insert_1(this_ptr, pos, n, c);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc
 * 2、std::string::insert(size_t pos, const char* s)
 */
STRING_HOOKER_HOOK_DEF(std::string&, insert_2, std::string *this_ptr, size_t pos, const char* s) {
    std::string &ret = string_hooker_orig_insert_2(this_ptr, pos, s);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertENS_11__wrap_iterIPKcEEc
 * 2、std::string::insert(std::__ndk1::__wrap_iter<const char*>, char)
 */
STRING_HOOKER_HOOK_DEF(std::string&, insert_3, std::__ndk1::__wrap_iter<const char*> iter, char c) {
    std::string &ret = string_hooker_orig_insert_3(iter, c);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmRKS5_mm
 * 2、std::string::insert(size_t pos, const std::string& str, size_t subpos, size_t sublen)
 */
STRING_HOOKER_HOOK_DEF(std::string&, insert_4, std::string *this_ptr, size_t pos, const std::string& str, size_t subpos, size_t sublen) {
    std::string &ret = string_hooker_orig_insert_4(this_ptr, pos, str, subpos, sublen);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}

/**
 * 1、_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKcm
 * 2、std::string::insert(size_t pos, const char* s, size_t n)
 */
STRING_HOOKER_HOOK_DEF(std::string&, insert_5, std::string *this_ptr, size_t pos, const char* s, size_t n) {
    std::string &ret = string_hooker_orig_insert_5(this_ptr, pos, s, n);
    DL_INFO
    IS_MATCH
        WRITE(ret, info);
    }
    return ret;
}



void hook_string(const char *path) {
    LOGE("start hook_string %s", path)
    auto handler = xdl_open(path, XDL_DEFAULT);
    void *sym;
    bool ret;

    //append
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendERKS5_mm",
            append_1)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE23__append_forward_unsafeINS_11__wrap_iterIPcEEEERS5_T_SB_",
            append_2)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEmc",
            append_3
    )
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKc",
            append_4)

    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE23__append_forward_unsafeIPcEERS5_T_S9_",
            append_5)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6appendEPKcm",
            append_6)

    //wappend
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6appendEmw",
            wappend_1)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE23__append_forward_unsafeIPwEERS5_T_S9_",
            wappend_2)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6appendEPKwm",
            wappend_3)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6appendEPKw",
            wappend_4)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6appendERKS5_mm",
            wappend_5)

    //construction(构造方法)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_mmRKS4_",
            construction_1)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_RKS4_",
            construction_2)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC1ERKS5_",
            construction_3)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEC2IDnEEPKc",
            construction_4)


    // std::string::assign
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignERKS5_mm",
            assign_1)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEmc",
            assign_2)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKcm",
            assign_3)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6assignEPKc",
            assign_4)

    //resize
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIwNS_11char_traitsIwEENS_9allocatorIwEEE6resizeEmw",
            resize_wchar)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6resizeEmc",
            resize_char)


    //std::string destructor 析构函数
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEED2Ev",
            basic_string_dtor)


    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7replaceEmmPKcm",
            replace_1)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7replaceEmmRKS5_mm",
            replace_2)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7replaceEmmmc",
            replace_3)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE7replaceEmmPKc",
            replace_4)

    // std::string::insert
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmmc",
            insert_1)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKc",
            insert_2)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertENS_11__wrap_iterIPKcEEc",
            insert_3)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmRKS5_mm",
            insert_4)
    StringHookerSym(
            "_ZNSt6__ndk112basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEE6insertEmPKcm",
            insert_5)

    xdl_close(handler);
}

void StringHooker::init(JNIEnv *env,
                        bool hookAll,
                        const std::list<std::string> &forbid_list,
                        const std::list<std::string> &filter_list,
                        std::ofstream *os) {
    isHookAll = hookAll;
    LOGE("start StringHooker trace is hook all %s", isHookAll ? "true" : "false")
    for (const std::string &str: forbid_list) {
        LOGE("start StringHooker trace forbid_list %s", str.c_str())
    }

    //copy orig list
    forbidSoList = std::list<string>(forbid_list);
    filterSoList = std::list<string>(filter_list);
    if (os != nullptr) {
        isSave = true;
        traceOs = os;
    }
    class StringHookerCallBack : public ZhenxiRunTime::LinkerLoadCallBack {
        void loadBefore(const char *path) const {

        }

        void loadAfter(const char *path, const char *redirect_path, void *ret) const {
            for (auto filter_item: StringHooker::filterSoList) {
                if (strcmp(filter_item.c_str(), getFileNameForPath(path).c_str()) == 0) {
                    hook_string(path);
                }
            }
        }
    };
    ZhenxiRunTime::linkerHandler::init();
    ZhenxiRunTime::linkerHandler::addLinkerCallBack(new StringHookerCallBack());
    //init_string_handler();
}

[[maybe_unused]]
void StringHooker::stop() {
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




