//
// Created by zhenxi on 2021/5/16.
//


#include "logging.h"

#include "dobby.h"
#include "shadowhook.h"

#ifndef VMP_HOOKUTILS_H
#define VMP_HOOKUTILS_H

enum Hook_MODEL {
    /**
      * 采用dobby进行hook
      * https://github.com/jmpews/Dobby
    */
    HOOK_MODEL_DOBBY = 1,
    /**
       * 采用shadowhook进行hook
       * https://github.com/bytedance/android-inline-hook
    */
    HOOK_MODEL_SHADOWHOOK = 2
};




#define HOOK_DEF(ret, func, ...) \
  ret (*orig_##func)(__VA_ARGS__)=nullptr; \
  ret new_##func(__VA_ARGS__)

#define HOOK_ADDRES(base,offset,name) bool is##name = HookUtils::Hooker((char *) il2cpp_base + offset,\
                                (void *) new_##name, \
                                (void **) &orig_##name);                                         \
                                if(is##name){                                                    \
                                   LOGE(#name)     \
                                } \

#define HOOK_SYM(sym,name) \
bool is##name = HookUtils::Hooker(sym,\
                                (void *) new_##name, \
                                (void **) &orig_##name);                                         \
                                if(is##name){                                                    \
                                   LOG(INFO) << #name << " hook sym success !!!!!";     \
                                } \


#define HOOKER(str_sym,name)       \
void *sym##name = xdl_sym(handle, str_sym, nullptr); \
if(sym##name == nullptr){ \
    sym##name = xdl_dsym(handle, str_sym, nullptr); \
    if( sym##name == nullptr) {           \
       LOG(ERROR) << #name <<" hook sym fail , "<< #str_sym<< " sym not find !!!!!"; \
       return;                             \
    }                              \
   \
} \
HOOK_SYM(sym##name,name)

class HookUtils {
public:
    static void setHookerModle(Hook_MODEL modle);
    static bool Hooker(void *dysym, void *repl, void **org,const std::string& tag);

    static bool Hooker(void *dysym, void *repl, void **org);

    static bool Hooker(void *handler, const char *dysym, void *repl, void **org);

    static bool Hooker(void *dysym, void *repl, void **org, const char *dynSymName);

    static bool Hooker(const char *libName, const char *dysym, void *repl, void **org);

    static bool addTrampoline(void *dysym , dobby_instrument_callback_t pre_handler);

    static void startBranchTrampoline();

    static bool unHook(void *sym);

    static void *getSymCompatForHandler(void *handler,const char *symbol);
    static void *getSymCompat(const char *filepath,const char *symbol);
};

void hook_libc_function(void *handle, const char *symbol, void *new_func, void **old_func) ;

#endif //VMP_HOOKUTILS_H
