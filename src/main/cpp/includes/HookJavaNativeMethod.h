//
// Created by Zhenxi on 2024/3/29.
//

#ifndef RUNTIME_HOOKJAVANATIVEMETHOD_H
#define RUNTIME_HOOKJAVANATIVEMETHOD_H

#include "jni.h"

class HookJavaMethod{
    public:
        static void* getJavaNativeMethodPtr(JNIEnv * env,jclass clazz,jmethodID method);
};
#endif //RUNTIME_HOOKJAVANATIVEMETHOD_H
