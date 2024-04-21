//
// Created by zhenxi on 2021/5/16.
//

#include <list>

#include "HookUtils.h"



#include "xdl.h"
#include "logging.h"
#include "ZhenxiLog.h"

using namespace std;
/**
 * 当前hook utils的hook模式
 */
static Hook_MODEL HOOKUTILS_MODEL = Hook_MODEL::HOOK_MODEL_DOBBY;

/**
 * shadowhook
 * 在做unhook的时候需要使用返回值进行unhook
 */
struct hook_addr_info{
    void* hook_orig;
    void* hook_ret;
    int hook_moudle;
};
/**
 * 保存全部hook的地址,防止某一个方法被多次Hook
 */
static list<hook_addr_info*> *hookedList  = new list<hook_addr_info*>();

static bool isInted = false;

/**
 * 取消函数Hook
 * @param sym  被Hook函数地址
 */
bool HookUtils::unHook(void *sym) {
    if(sym == nullptr){
        LOGE("HookUtils unHook sym == null ")
        return false;
    }

    hook_addr_info* info = nullptr;
    for (hook_addr_info* ptr : *hookedList) {
        if(ptr->hook_moudle == Hook_MODEL::HOOK_MODEL_DOBBY){
            if(ptr->hook_orig == sym){
                info = ptr;
                break;
            }
        } else if(ptr->hook_moudle == Hook_MODEL::HOOK_MODEL_SHADOWHOOK){
            if(ptr->hook_ret == sym){
                info = ptr;
                break;
            }
        }
    }
    if(info == nullptr){
        LOG(ERROR) << "HookUtils unHook info == nullptr  !!!" ;
        LOG(ERROR) << "HookUtils unHook info == nullptr  @@@" ;
        LOG(ERROR) << "HookUtils unHook info == nullptr  ###" ;
        return false;
    }
    bool ret = false;
    if(HOOKUTILS_MODEL == HOOK_MODEL_DOBBY){
        ret = DobbyDestroy(info->hook_orig) == RT_SUCCESS;
    }else if(HOOKUTILS_MODEL == HOOK_MODEL_SHADOWHOOK){
        ret = shadowhook_unhook(info->hook_ret) == SHADOWHOOK_ERRNO_OK;
    }
    hookedList->remove(info);
    return ret;
}

#define PUT_PTR(orig,ret,moudle) \
        hook_addr_info *info  = new hook_addr_info(); \
        info->hook_orig = orig;\
        info->hook_ret = ret;    \
        info->hook_moudle = moudle;     \
        hookedList->push_back(info); \
        //LOGI("hook utils success orig -> %p ret -> %p  [%s] ",orig,ret,moudle == Hook_MODEL::HOOK_MODEL_DOBBY?"dobby":"shadowhook" ) \


[[maybe_unused]]
void  HookUtils::startBranchTrampoline(){
    dobby_enable_near_branch_trampoline();
}
[[maybe_unused]]
void HookUtils::setHookerModle(Hook_MODEL modle){
    HOOKUTILS_MODEL = modle;
}
/**
 * Hook的整体封装,这个方法可以切换别的Hook框架
 * 先尝试用DobbyHook 如果Hook失败的话用InlineHook二次尝试
 *
 * @param dysym  被Hook函数地址
 * @param repl   替换函数
 * @param org    原始函数指针
 * @return 是否Hook成功
 */
bool HookUtils::Hooker(void *dysym, void *newrep, void **org) {
    if (dysym == nullptr) {
        LOG(ERROR) << "Hooker hook org == null ";
        return false;
    }
    //init hook for shadowhook
    if(!isInted){
        if(HOOKUTILS_MODEL == Hook_MODEL::HOOK_MODEL_SHADOWHOOK){
            //a function can be hooked multiple times
            //SHADOWHOOK_MODE_SHARED = 0
            //a function can only be hooked once, and hooking again will report an error
            //SHADOWHOOK_MODE_UNIQUE = 1
            auto ret =shadowhook_init(shadowhook_mode_t::SHADOWHOOK_MODE_UNIQUE,
                                      false
            );
            if(ret == SHADOWHOOK_ERRNO_OK) {
                isInted = true;
            } else{
                LOG(ERROR) << "shadowhook_init init error  "<<shadowhook_to_errmsg(ret);
            }
        }else if(HOOKUTILS_MODEL == Hook_MODEL::HOOK_MODEL_DOBBY){
            //dobby no init
            isInted = true;
        }
    }
    //如果这个地址已经被Hook了 。也有可能返回失败 。dobby 会提示 already been hooked 。
    for (hook_addr_info* ptr: *hookedList) {
        if (ptr->hook_orig == dysym) {
            //如果保存了这个地址,说明之前hook成功过,我们也认为hook成功
            return true;
        }
    }

    string errorMsg = {};
    if(HOOKUTILS_MODEL == Hook_MODEL::HOOK_MODEL_DOBBY){
        //hook used dobby
        bool ret = DobbyHook(dysym,
                        reinterpret_cast<dobby_dummy_func_t>(newrep),
                        reinterpret_cast<dobby_dummy_func_t *>(org)
                        )== RT_SUCCESS;
        if(ret){
            PUT_PTR(dysym, nullptr,Hook_MODEL::HOOK_MODEL_DOBBY)
            return true;
        } else{
            LOG(ERROR) << "HookUtils::Hooker dobby hook ret == false";
        }
    }else if(HOOKUTILS_MODEL == Hook_MODEL::HOOK_MODEL_SHADOWHOOK){
        void* sub = shadowhook_hook_func_addr(dysym,
                                        newrep,
                                        org
        );
        if(sub!= nullptr){
            PUT_PTR(dysym,sub,Hook_MODEL::HOOK_MODEL_SHADOWHOOK)
            return true;
        }else{
            int err_num = shadowhook_get_errno();
            errorMsg = shadowhook_to_errmsg(err_num);
            LOG(ERROR) << "HookUtils::Hooker shadowhook hook ret == false";
        }
    }
    Dl_info info;
    dladdr(dysym,&info);
    LOG(ERROR) << "HookUtils::Hooker hook error"
                  " "<<info.dli_fname<<" "<<info.dli_sname
                  <<" error msg "<<errorMsg;

    return false;

}

bool HookUtils::Hooker(void *handler, const char *dysym, void *repl, void **org) {
    void *sym = getSymCompatForHandler(handler, dysym);
    if (sym == nullptr) {
        LOG(ERROR) << "HookUtils hook sym == null    " << dysym;
        return false;
    }
    bool isSuccess = Hooker(sym, repl, org);
    if (!isSuccess) {
        LOG(ERROR) << "HookUtils hook sym error   " << dysym;
    }
    return isSuccess;
}

/**
 *
 * @param dysym  被Hook函数地址
 * @param repl   替换函数
 * @param org    原始函数指针
 * @param dynSymName  函数的符号,主要为了在失败时候方便打印那个函数失败了
 * @return 是否Hook成功
 */
bool HookUtils::Hooker(void *dysym,
                       void *repl,
                       void **org,
                       [[maybe_unused]] const char *dynSymName) {
    if (Hooker(dysym, repl, org)) {
        return true;
    }
    return false;
}
/**
 * 对一个方法进行插装
 * dobby_instrument_callback_t
 */
[[maybe_unused]]
bool HookUtils::addTrampoline(void *dysym,dobby_instrument_callback_t pre_handler) {
    return DobbyInstrument(dysym,pre_handler) == RT_SUCCESS;
}
/**
 *
 * @param libName  So的路径
 * @param dysym  函数的符号
 * @param repl   替换函数
 * @param org    原始函数指针
 * @return 是否Hook成功
 */
bool HookUtils::Hooker(const char *libName, const char *dysym, void *repl, void **org) {
    void *sym = getSymCompat(libName, dysym);
    if (sym == nullptr) {
        LOG(ERROR) << "Hooker  get sym error " << dysym;
        return false;
    }
    bool isSucess = Hooker(sym, repl, org);
    if (!isSucess) {
        LOG(ERROR) << "Hooker hook error " << libName << " " << dysym;
    }
    return isSucess;
}



/**
 * 处理libc相关
 */
void hook_libc_function(void *handle, const char *symbol, void *new_func, void **old_func) {
    void *addr = dlsym(handle, symbol);
    if (addr == nullptr) {
        LOGE("hook_function_libc not found symbol : %s", symbol)
        return;
    }
    if (!HookUtils::Hooker(addr, new_func, old_func)) {
        LOGE(">>>>>>>>>>> io  hook %s fail !", symbol)
    }
}