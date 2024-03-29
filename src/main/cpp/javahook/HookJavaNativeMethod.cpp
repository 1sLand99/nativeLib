//
// Created by Zhenxi on 2024/3/29.
//

#include "HookJavaNativeMethod.h"
#include "ZhenxiLog.h"
#include "xdl.h"
#include "libpath.h"

typedef void* (*GetEntryPointFromJniFunc)(void*);

void* getArtMethodPtr(void* art_method){
    void *GetEntryPointFromJniPtr = getSymCompat(getlibArtPath().c_str(), "");
    if(GetEntryPointFromJniPtr == nullptr){
        LOGE("HookJavaMethod::getArtMethodPtr GetEntryPointFromJniPtr == nullptr ")
        return nullptr;
    }
    GetEntryPointFromJniFunc funcPointer = (GetEntryPointFromJniFunc)GetEntryPointFromJniPtr;
    if(funcPointer == nullptr){
        LOGE("HookJavaMethod::getArtMethodPtr funcPointer == nullptr ")
        return nullptr;
    }
    return funcPointer(art_method);
}

/**
 *  1、先去取art_meth结构体的地址。
 *  可以通过getField(Method.class, "artMethod")获取art_method地址。
 *  2、在通过
 *  art_method -> void* GetEntryPointFromJni()取native的注册地址
 *  3、使用inlinehook
 */
void* HookJavaMethod::getJavaNativeMethodPtr(JNIEnv * env,jclass clazz,jmethodID method){
    if(method == nullptr){
        LOGE("HookJavaMethod::getJavaNativeMethodPtr method == nullptr")
        return nullptr;
    }
    jfieldID fieldId = env->GetFieldID(clazz, "artMethod", "J");
    if(fieldId == nullptr){
        LOGE("HookJavaMethod::getJavaNativeMethodPtr fieldId == nullptr ")
        return nullptr;
    }
    jlong ptr = env->GetLongField((jobject)method, fieldId);
    if(ptr == 0){
        LOGE("HookJavaMethod::getJavaNativeMethodPtr ptr == 0 ")
        return nullptr;
    }
    return getArtMethodPtr((void*)ptr);
}