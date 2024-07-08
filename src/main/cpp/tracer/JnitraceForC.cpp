

#include "JnitraceForC.h"

#include "mylibc.h"
#include "HookUtils.h"
#include "ZhenxiLog.h"
#include "TracerBase.h"


jmethodID Jnitrace::object_method_id_toString = nullptr;
/**
 * 在打印枚举的时候发现可能会被当成object去打印
 * 导致报错
 * JNI DETECTED ERROR IN APPLICATION: use of invalid jobject
 * 尝试在Java去打印
 */
jclass Jnitrace::auxiliary_clazz = nullptr;
jmethodID Jnitrace::auxiliary_clazz_method_id_toString = nullptr;


#define HOOK_JNITRACE(env, func) \
    bool func##hook_success = HookUtils::Hooker(    \
      (void*)(env)->functions->func,\
      (void*)new_##func,\
      (void**)&orig_##func );    \
      if(!func##hook_success) {  \
         LOG(INFO) <<"jnitrace hook error "<<#func; \
      } \

#define JNITRACE_HOOK_DEF(ret, func, ...) \
  ret (*orig_##func)(__VA_ARGS__); \
  ret new_##func(__VA_ARGS__)

#define JNI_HOOK_DEF(ret, func, ...)    \
  ret (*orig_##func)(JNIEnv *env, jobject obj, jmethodID jmethodId,...) = nullptr;  \
  ret new_##func(__VA_ARGS__) {         \
  //if(orig_##func == nullptr){LOG(INFO) <<"jnitrace hook error ,org sym == null "<<#func;}  \
  //LOG(INFO) << #func<<"("#__VA_ARGS__")" ;\

#define GET_JOBJECT_INFO(env, obj, methodname) \
            GET_ADDRESS                                   \
            Jnitrace::getJObjectInfo(env,obj,string("(")+TracerBase::getFileNameForPath(info.dli_fname)+")"+ methodname+"<"+address+">"+"("+getprogname()+")");\


#define GET_METHOD_INFO_ARGS(env, obj, methodid, args, isStatic) Jnitrace::getArgsInfo(env,obj,methodid,args,isStatic);

//jobject CallObjectMethod(JNIEnv*, jobject, jmethodID, va_list args);
JNI_HOOK_DEF(jobject, CallObjectMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jobject ret = orig_CallObjectMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallObjectMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        Jnitrace::getJObjectInfoInternal(env, ret, "result object :", true, nullptr);
        return ret;
    }
    return orig_CallObjectMethodV(env, obj, jmethodId, args);
}

//void CallVoidMethod(jobject obj, jmethodID methodID, va_list args)
JNI_HOOK_DEF(void, CallVoidMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        GET_JOBJECT_INFO(env, obj, "CallVoidMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)

    }
    return orig_CallVoidMethodV(env, obj, jmethodId, args);
}

JNI_HOOK_DEF(jboolean, CallBooleanMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)

    DL_INFO
    IS_MATCH
        jboolean ret = orig_CallBooleanMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallBooleanMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        WRITE(string("invoke method result Boolean : ").append(
                (ret == JNI_TRUE ? "true" : "false")).append("\n"));
        return ret;
    }
    return orig_CallBooleanMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jbyte, CallByteMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jbyte ret = orig_CallByteMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallByteMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        WRITE(string("result Byte : ").append(to_string(ret)).append("\n"));
        return ret;
    }

    return orig_CallByteMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jchar, CallCharMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jchar ret = orig_CallCharMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallCharMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        WRITE(string("result Char : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallCharMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jshort, CallShortMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jshort ret = orig_CallShortMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallShortMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        WRITE(string("result Short : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallShortMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jint, CallIntMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jint ret = orig_CallIntMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallIntMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        WRITE(string("result Int : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallIntMethodV(env, obj, jmethodId, args);

}


JNI_HOOK_DEF(jlong, CallLongMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jlong ret = orig_CallLongMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallLongMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        WRITE(string("result Long : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallLongMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jfloat, CallFloatMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jfloat ret = orig_CallFloatMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallFloatMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        WRITE(string("result Float : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallFloatMethodV(env, obj, jmethodId, args);

}

JNI_HOOK_DEF(jdouble, CallDoubleMethodV, JNIEnv *env, jobject obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jdouble ret = orig_CallDoubleMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallDoubleMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        WRITE(string("result Double : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallDoubleMethodV(env, obj, jmethodId, args);
}



//------------------ static ------------------------------------------------------------

JNI_HOOK_DEF(jobject, CallStaticObjectMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jobject ret = orig_CallStaticObjectMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticObjectMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        Jnitrace::getJObjectInfoInternal(env, ret, "result object :", true, nullptr);
        return ret;
    }
    return orig_CallStaticObjectMethodV(env, obj, jmethodId, args);
}

//void CallVoidMethod(jobject obj, jmethodID methodID, va_list args)
JNI_HOOK_DEF(void, CallStaticVoidMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        GET_JOBJECT_INFO(env, obj, "CallStaticVoidMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)

    }
    return orig_CallStaticVoidMethodV(env, obj, jmethodId, args);
}

JNI_HOOK_DEF(jboolean, CallStaticBooleanMethodV, JNIEnv *env, jclass obj,
             jmethodID jmethodId,
             va_list args)

    DL_INFO
    IS_MATCH

        jboolean ret = orig_CallStaticBooleanMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticBooleanMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        WRITE(string("result Boolean : ").append(
                (ret == JNI_TRUE ? "true" : "false")).append(
                "\n"));
        return ret;
    }

    return orig_CallStaticBooleanMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jbyte, CallStaticByteMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jbyte ret = orig_CallStaticByteMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticByteMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        WRITE(string("result Byte : ").append(to_string(ret)).append("\n"));
        return ret;
    }


    return orig_CallStaticByteMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jchar, CallStaticCharMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jchar ret = orig_CallStaticCharMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticCharMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        WRITE(string("result Char : ").append(to_string(ret)).append("\n"));
        return ret;

    }
    return orig_CallStaticCharMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jshort, CallStaticShortMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jshort ret = orig_CallStaticShortMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticShortMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        WRITE(string("result Short : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallStaticShortMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jint, CallStaticIntMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jint ret = orig_CallStaticIntMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticIntMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        WRITE(string("result Int : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallStaticIntMethodV(env, obj, jmethodId, args);

}


JNI_HOOK_DEF(jlong, CallStaticLongMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jlong ret = orig_CallStaticLongMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticLongMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        WRITE(string("result Long : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallStaticLongMethodV(env, obj, jmethodId, args);
}


JNI_HOOK_DEF(jfloat, CallStaticFloatMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jfloat ret = orig_CallStaticFloatMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticFloatMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        WRITE(string("result Float : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallStaticFloatMethodV(env, obj, jmethodId, args);

}


JNI_HOOK_DEF(jdouble, CallStaticDoubleMethodV, JNIEnv *env, jclass obj, jmethodID jmethodId,
             va_list args)
    DL_INFO
    IS_MATCH
        jdouble ret = orig_CallStaticDoubleMethodV(env, obj, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "CallStaticDoubleMethodV")
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, true)
        WRITE(string("result Double : ").append(to_string(ret)).append("\n"));
        return ret;
    }
    return orig_CallStaticDoubleMethodV(env, obj, jmethodId, args);
}


void Jnitrace::getJObjectInfo(JNIEnv *env, jobject obj, const string &methodname) {
    if (obj == nullptr) {
        return;
    }
    if (env->ExceptionCheck()) {
        //exception return
        return;
    }
    const string temptag =
            "<<<<<------------------" + methodname + " --------------------->>>>>\n";
    WRITE(temptag, true);
    Jnitrace::getJObjectInfoInternal(env, obj, "invoke this object", true, nullptr);
}

/**
 * 打印一个obj类型的信息
 *
 * @param env
 * @param obj 需要打印的obj
 * @param message 输出变量的信息
 * @param isPrintClassinfo 是否输出Class信息
 * @param classinfo classinfo信息
 */
void Jnitrace::getJObjectInfoInternal(JNIEnv *env,
                                      jobject obj,
                                      string message,
                                      bool isPrintClassinfo,
                                      const char *classInfo) {

    if (obj == nullptr) {
        return;
    }
    if (classInfo == nullptr) {
        jclass objClass = env->GetObjectClass(obj);
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
        classInfo = getJObjectClassInfo(env, objClass).c_str();
    }
    if (classInfo == nullptr) {
        return;
    }
    const char *ret = nullptr;
    //需要先排除基础类型
    if (my_strcmp(classInfo, "boolean") == 0 ||
        my_strcmp(classInfo, "java.lang.Boolean") == 0) {
        jclass clazz = env->FindClass("java/lang/Boolean");
        jmethodID id = env->GetStaticMethodID(clazz, "toString", "(Z)Ljava/lang/String;");
        auto jStr = (jstring) env->CallStaticObjectMethod(clazz, id, obj);
        ret = env->GetStringUTFChars(jStr, nullptr);
    }
    else if (my_strcmp(classInfo, "byte") == 0 ||
               my_strcmp(classInfo, "java.lang.Byte") == 0) {
        jclass clazz = env->FindClass("java/lang/Byte");
        jmethodID id = env->GetStaticMethodID(clazz, "toString", "(B)Ljava/lang/String;");
        auto jStr = (jstring) env->CallStaticObjectMethod(clazz, id, obj);
        ret = env->GetStringUTFChars(jStr, nullptr);
    }
    else if (my_strcmp(classInfo, "char") == 0 ||
               my_strcmp(classInfo, "java.lang.Character") == 0) {
        jclass clazz = env->FindClass("java/lang/Character");
        jmethodID id = env->GetStaticMethodID(clazz, "toString", "(C)Ljava/lang/String;");
        auto jStr = (jstring) env->CallStaticObjectMethod(clazz, id, obj);
        ret = env->GetStringUTFChars(jStr, nullptr);

    }
    else if (my_strcmp(classInfo, "short") == 0 ||
               my_strcmp(classInfo, "java.lang.Short") == 0) {
        jclass clazz = env->FindClass("java/lang/Short");
        jmethodID id = env->GetStaticMethodID(clazz, "toString", "(S)Ljava/lang/String;");
        auto jStr = (jstring) env->CallStaticObjectMethod(clazz, id, obj);
        ret = env->GetStringUTFChars(jStr, nullptr);

    }
    else if (my_strcmp(classInfo, "int") == 0 ||
               my_strcmp(classInfo, "java.lang.Integer") == 0) {
        jclass clazz = env->FindClass("java/lang/Integer");
        jmethodID id = env->GetStaticMethodID(clazz, "toString", "(I)Ljava/lang/String;");
        auto jStr = (jstring) env->CallStaticObjectMethod(clazz, id, obj);
        ret = env->GetStringUTFChars(jStr, nullptr);

    }
    else if (my_strcmp(classInfo, "float") == 0 ||
               my_strcmp(classInfo, "java.lang.Float") == 0) {
        jclass clazz = env->FindClass("java/lang/Float");
        jmethodID id = env->GetStaticMethodID(clazz, "toString", "(F)Ljava/lang/String;");
        auto jStr = (jstring) env->CallStaticObjectMethod(clazz, id, obj);
        ret = env->GetStringUTFChars(jStr, nullptr);

    }
    else if (my_strcmp(classInfo, "double") == 0 ||
               my_strcmp(classInfo, "java.lang.Double") == 0) {
        jclass clazz = env->FindClass("java/lang/Double");
        jmethodID id = env->GetStaticMethodID(clazz, "toString", "(D)Ljava/lang/String;");
        auto jStr = (jstring) env->CallStaticObjectMethod(clazz, id, obj);
        ret = env->GetStringUTFChars(jStr, nullptr);

    }
    else if (my_strcmp(classInfo, "long") == 0 ||
               my_strcmp(classInfo, "java.lang.Long") == 0) {
        jclass clazz = env->FindClass("java/lang/Long");
        jmethodID id = env->GetStaticMethodID(clazz, "toString", "(J)Ljava/lang/String;");
        auto jStr = (jstring) env->CallStaticObjectMethod(clazz, id, obj);
        ret = env->GetStringUTFChars(jStr, nullptr);
    }
    else if (my_strstr(classInfo, "[")) {
        //数组类型需要特殊处理,非数组直接打印
        auto arg = (jobjectArray) obj;
        //数组类型参数
        jstring argJstr;
        //byte数组 特殊处理
        if (my_strcmp(classInfo, "[B") == 0) {
            jclass strclazz = env->FindClass("java/lang/String");
            jstring utf = env->NewStringUTF("UTF-8");
            jmethodID strInit = env->GetMethodID(strclazz, "<init>",
                                                 "([BLjava/lang/String;)V");
            argJstr = (jstring) (env->NewObject(strclazz, strInit, arg, utf));
            env->DeleteLocalRef(utf);
            env->DeleteLocalRef(strclazz);
        } else {
            //其他的则调用Arrays.toString 处理
            jclass ArrayClazz = env->FindClass("java/util/Arrays");
            jmethodID methodid = nullptr;
            if (my_strcmp(classInfo, "[F") == 0) {
                methodid = env->GetStaticMethodID(ArrayClazz, "toString",
                                                  "([F)Ljava/lang/String;");
            } else if (my_strcmp(classInfo, "[I") == 0) {
                methodid = env->GetStaticMethodID(ArrayClazz, "toString",
                                                  "([I)Ljava/lang/String;");
            } else if (my_strcmp(classInfo, "[S") == 0) {
                methodid = env->GetStaticMethodID(ArrayClazz, "toString",
                                                  "([S)Ljava/lang/String;");
            } else if (my_strcmp(classInfo, "[J") == 0) {
                methodid = env->GetStaticMethodID(ArrayClazz, "toString",
                                                  "([J)Ljava/lang/String;");
            } else if (my_strcmp(classInfo, "[D") == 0) {
                methodid = env->GetStaticMethodID(ArrayClazz, "toString",
                                                  "([D)Ljava/lang/String;");
            } else if (my_strcmp(classInfo, "[C") == 0) {
                methodid = env->GetStaticMethodID(ArrayClazz, "toString",
                                                  "([C)Ljava/lang/String;");
            } else if (my_strcmp(classInfo, "[Z") == 0) {
                methodid = env->GetStaticMethodID(ArrayClazz, "toString",
                                                  "([Z)Ljava/lang/String;");
            } else {
                //这个需要用object类型
                methodid =
                        env->GetStaticMethodID(ArrayClazz,
                                               "toString",
                                               "([Ljava/lang/Object;)Ljava/lang/String;");
            }

            argJstr = (jstring) (env->CallStaticObjectMethod(ArrayClazz, methodid, arg));
        }
        if (argJstr != nullptr) {
            //上面的逻辑主要是为了处理argJstr的赋值
            ret = env->GetStringUTFChars(argJstr, nullptr);
        }
    }
    else {
        //其他任何类型直接toString
        ret = getJObjectToString(env, obj,classInfo).c_str();
    }
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
    }
    if (ret == nullptr) {
        write("jnitrace get obj info error " + string(classInfo), false);
        return;
    }
    if (my_strlen(ret) > 0) {
        if (isPrintClassinfo) {
            string basicString = message.append("  [").append(classInfo).append("]  ").append(
                    ret).append("\n");
            write(basicString);
        } else {
            string basicString = message.append("  ").append(ret).append("\n");
            write(basicString);
        }
    }
}


void Jnitrace::getArgsInfo(JNIEnv *env, jobject obj, jmethodID jmethodId,
                           va_list args, bool isStatic
) {

    if (obj == nullptr) {
        return;
    }
    if (env->ExceptionCheck()) {
        //exception return
        return;
    }
    if (jmethodId == nullptr) {
        return;
    }

    jclass pJclass = env->GetObjectClass(obj);

    //获取被调用的方法信息
    jobject invokeMethod = env->ToReflectedMethod(pJclass, jmethodId, isStatic);

    //打印被调用方法信息,只需要调用他的toString
    getJObjectInfoInternal(env, invokeMethod, "invoke method", false, nullptr);


    jclass objclass = env->GetObjectClass(invokeMethod);

    jmethodID parCountId = env->GetMethodID(objclass, "getParameterCount", "()I");
    // 获取方法长度
    jint size = env->CallIntMethod(invokeMethod, parCountId);

    if (size == 0) {
        //长度等于0直接return,不打印参数信息
        return;
    }

    jmethodID method_id_getTypes = env->GetMethodID(
            objclass,
            "getParameterTypes",
            "()[Ljava/lang/Class;");

    auto objectArray = (jobjectArray) (env->CallObjectMethod(invokeMethod,
                                                             method_id_getTypes));
    for (int i = 0; i < size; i++) {
        auto argobj = env->GetObjectArrayElement(objectArray, i);
        if (argobj == nullptr) {
            continue;
        }
        char argsInfo[20];
        sprintf(argsInfo, "args index [%d] ", i);
        //具体每个类型的class
        auto classInfo = getJObjectClassInfo(env, argobj);
        jobject arg = va_arg(args, jobject);
        getJObjectInfoInternal(env, arg, argsInfo, true, classInfo.c_str());
    }
    //end
    va_end(args);

}

/*
 * get obj class info
 */
string Jnitrace::getJObjectClassInfo(JNIEnv *env, jobject obj) {
    if (obj == nullptr) {
        return "";
    }
    //如果目标Apk开启了子线程,这块可能会导致Env没有被Attch
    jclass clazz = env->FindClass("java/lang/Class");
    if (clazz == nullptr) {
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
        return "";
    }
    jmethodID method_id_getName = env->GetMethodID(clazz, "getName",
                                                   "()Ljava/lang/String;");
    if (method_id_getName == nullptr) {
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
        return "";
    }
    //print class info
    auto getClassName_ret = (jstring) (env->CallObjectMethod(obj, method_id_getName));


    const char *getClass = env->GetStringUTFChars(getClassName_ret, nullptr);

    //free
    env->ReleaseStringUTFChars(getClassName_ret, getClass);
    env->DeleteLocalRef(getClassName_ret);

    return const_cast<char *>(getClass);
}

/*
 * get obj to string
 */
string Jnitrace::getJObjectToString(JNIEnv *env, jobject obj,const char * classInfo) {
    if(obj == nullptr) {
        return {};
    }
    jstring str;
    if(auxiliary_clazz!= nullptr) {
        if(auxiliary_clazz_method_id_toString == nullptr) {
            auxiliary_clazz_method_id_toString = env->GetStaticMethodID(auxiliary_clazz,
                                                                       "obj2str",
                                                                       "(Ljava/lang/Object;)Ljava/lang/String;");
        }
        str = (jstring) env->CallStaticObjectMethod(auxiliary_clazz,
                                                    auxiliary_clazz_method_id_toString,obj
                                              );
    }else {
        if (object_method_id_toString == nullptr) {
            object_method_id_toString =
                    env->GetMethodID(env->FindClass("java/lang/Object"),
                                     "toString",
                                     "()Ljava/lang/String;");
        }
        if(classInfo!= nullptr) {
            if (my_strcmp(classInfo, "android.graphics.Shader$TileMode") == 0||
                my_strcmp(classInfo, "android.graphics.Paint") == 0
                ) {
                return "";
            }
        }
        str = (jstring) env->CallObjectMethod(obj,
                                              object_method_id_toString
        );
    }
    const char *rawString = env->GetStringUTFChars(str, nullptr);
    std::string result(rawString);
    env->ReleaseStringUTFChars(str, rawString);
    return result;
}
//jstring NewStringUTF(const char* bytes)
JNITRACE_HOOK_DEF(jstring, NewStringUTF, JNIEnv *env, const char *bytes) {
    DL_INFO
    IS_MATCH
        GET_ADDRESS
        WRITE(string(string("<") + address + ">NewStringUTF : ").append(
                bytes == nullptr ? "" : bytes).append("\n"));
        return orig_NewStringUTF(env, bytes);

    }
    return orig_NewStringUTF(env, bytes);
}

//const char* GetStringUTFChars(jstring string, jboolean* isCopy)
JNITRACE_HOOK_DEF(const char*, GetStringUTFChars, JNIEnv *env, jstring argstring,
                  [[maybe_unused]] jboolean * isCopy) {
    DL_INFO
    IS_MATCH
        if (argstring == nullptr) {
            return nullptr;
        }
        const char *chars = orig_GetStringUTFChars(env, argstring, isCopy);
        if (chars == nullptr || strlen(chars) == 0) {
            return chars;
        }
        GET_ADDRESS
        WRITE(string("<") + address + string(">GetStringUTFChars : ").append(chars).append("\n"));
        return chars;

    }
    return orig_GetStringUTFChars(env, argstring, isCopy);
}

//jclass FindClass(const char* name)
JNITRACE_HOOK_DEF(jclass, FindClass, JNIEnv *env, const char *name) {
    DL_INFO
    IS_MATCH
        GET_ADDRESS
        WRITE(string("<") + address + string(">find class : ").append(
                name == nullptr ? "" : name).append("\n"));
        return orig_FindClass(env, name);

    }
    return orig_FindClass(env, name);
}



//jobject     (*NewObjectV)(JNIEnv*, jclass, jmethodID, va_list);
JNITRACE_HOOK_DEF(jobject, NewObjectV, JNIEnv *env, jclass clazz, jmethodID jmethodId,
                  va_list args) {
    DL_INFO
    IS_MATCH
        //当method id 错误时候可能null
        jobject obj = orig_NewObjectV(env, clazz, jmethodId, args);
        GET_JOBJECT_INFO(env, obj, "NewObjectV")
        //打印构造方法参数信息
        GET_METHOD_INFO_ARGS(env, obj, jmethodId, args, false)
        return obj;

    }
    return orig_NewObjectV(env, clazz, jmethodId, args);
}

//void SetByteArrayRegion(jbyteArray array, jsize start, jsize len,const jbyte* buf)
JNITRACE_HOOK_DEF(void, SetByteArrayRegion, JNIEnv *env,
                  jbyteArray array, jsize start, jsize len,
                  const jbyte *buf) {
    DL_INFO
    IS_MATCH
        orig_SetByteArrayRegion(env, array, start, len, buf);
        GET_ADDRESS
        Jnitrace::getJObjectInfoInternal(env, array,
                                         string("<") + address + ">SetByteArrayRegion java array, start->" +
                                         to_string(start) + " len -> " +
                                         to_string(len), false,
                                         nullptr);

    }
    orig_SetByteArrayRegion(env, array, start, len, buf);
}

JNITRACE_HOOK_DEF(void, GetByteArrayRegion, JNIEnv *env,
                  jbyteArray array, jsize start, jsize len,
                  jbyte *buf) {
    DL_INFO
    IS_MATCH
        orig_GetByteArrayRegion(env, array, start, len, buf);
        GET_ADDRESS
        Jnitrace::getJObjectInfoInternal(env, array,
                                         string("<") + address + ">GetByteArrayRegion java array, start->" +
                                         to_string(start) + " len -> " +
                                         to_string(len), false,
                                         nullptr);

    }
    orig_GetByteArrayRegion(env, array, start, len, buf);
}
//void  (*GetStringRegion)(JNIEnv*, jstring, jsize, jsize, jchar*);
JNITRACE_HOOK_DEF(void, GetStringRegion,
                  JNIEnv *env, jstring str, jsize start, jsize len, jchar *buf) {
    DL_INFO
    IS_MATCH
        orig_GetStringRegion(env, str, start, len, buf);
        GET_ADDRESS
        Jnitrace::getJObjectInfoInternal(env, str,
                                         string("<") + address + ">GetStringRegion java str, start->" +
                                         to_string(start) + " len -> " +
                                         to_string(len), false,
                                         nullptr);

    }
    orig_GetStringRegion(env, str, start, len, buf);
}

JNITRACE_HOOK_DEF(void, GetStringUTFRegion,
                  JNIEnv *env, jstring str, jsize start, jsize len, char *buf) {
    DL_INFO
    IS_MATCH
        orig_GetStringUTFRegion(env, str, start, len, buf);
        GET_ADDRESS
        Jnitrace::getJObjectInfoInternal(env, str,
                                         string("<") + address + ">GetStringUTFRegion java str, start->" +
                                         to_string(start) + " len -> " +
                                         to_string(len), false,
                                         nullptr);

    }
    orig_GetStringUTFRegion(env, str, start, len, buf);
}

//jobject     (*ToReflectedMethod)(JNIEnv*, jclass, jmethodID, jboolean);
JNITRACE_HOOK_DEF(jobject, ToReflectedMethod, JNIEnv *env, jclass clazz,
                  jmethodID jmethodId,
                  jboolean isStatic) {
    DL_INFO
    IS_MATCH
        jobject obj = orig_ToReflectedMethod(env, clazz, jmethodId, isStatic);
        if (obj != nullptr) {
            GET_ADDRESS
            Jnitrace::getJObjectInfoInternal(env, obj, string("<") + address + ">ToReflectedMethod result ",
                                             false,
                                             nullptr);
        }
        return obj;

    }
    return orig_ToReflectedMethod(env, clazz, jmethodId, isStatic);
}

//jmethodID FromReflectedMethod(jobject method)
JNITRACE_HOOK_DEF(jobject, FromReflectedMethod, JNIEnv *env, jobject method) {
    DL_INFO
    IS_MATCH
        GET_ADDRESS
        Jnitrace::getJObjectInfoInternal(env, method, string("<") + address + ">FromReflectedMethod arg ",
                                         false,
                                         nullptr);
        return orig_FromReflectedMethod(env, method);

    }
    return orig_FromReflectedMethod(env, method);
}


//jfieldID  (*GetFieldID)(JNIEnv*, jclass, const char*, const char*);
JNITRACE_HOOK_DEF(jfieldID, GetFieldID, JNIEnv *env, jclass clazz, const char *name,
                  const char *sig) {
    DL_INFO
    IS_MATCH
        jmethodID method_id_toString = env->GetMethodID(
                env->FindClass("java/lang/Object"), "toString", "()Ljava/lang/String;");
        auto toString_ret = (jstring) (env->CallObjectMethod(clazz,
                                                             method_id_toString));
        const char *toString = env->GetStringUTFChars(toString_ret, nullptr);
        //LOG(INFO) << "get field info  :  " << toString << "  " << name << "  " << sig;
        //os << "get field info  :  " << toString << "  " << name << "  " << sig << "\n";
        GET_ADDRESS
        WRITE(string(string("<") + address + ">get field info  : ").append(
                        toString == nullptr ? "" : toString)
                                .append(" name-> ").append(name == nullptr ? "" : name)
                                .append(" sign -> ").append(sig == nullptr ? "" : sig).append(
                        "\n"));
        return orig_GetFieldID(env, clazz, name, sig);

    }
    return orig_GetFieldID(env, clazz, name, sig);
}

//jfieldID    (*GetStaticFieldID)(JNIEnv*, jclass, const char*,const char*);
JNITRACE_HOOK_DEF(jfieldID, GetStaticFieldID, JNIEnv *env, jclass clazz, const char *name,
                  const char *sig) {
    DL_INFO
    IS_MATCH
        jmethodID method_id_toString = env->GetMethodID(
                env->FindClass("java/lang/Object"), "toString", "()Ljava/lang/String;");
        auto toString_ret = (jstring) (env->CallObjectMethod(clazz,
                                                             method_id_toString));
        const char *toString = env->GetStringUTFChars(toString_ret, nullptr);
        //LOG(INFO) << "get static field info  :  " << toString << "  " << name << "  " << sig;
        //os << "get static field info  :  " << toString << "  " << name << "  " << sig << "\n";
        GET_ADDRESS
        WRITE(string(string("<") + address + ">get static field info  : ").append(
                        toString == nullptr ? "" : toString)
                                .append(" ").append(name).append(" ").append(sig).append("\n"));
        return orig_GetStaticFieldID(env, clazz, name, sig);

    }
    return orig_GetStaticFieldID(env, clazz, name, sig);
}

//--------------------------getField----------------------------
//jobject     (*GetObjectField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jobject, GetObjectField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetObjectField")
        jobject ret = orig_GetObjectField(env, argObj, jfieldId);
        if (ret != nullptr) {
            GET_ADDRESS
            Jnitrace::getJObjectInfoInternal(env, ret,
                                             string("<") + address + ">GetObjectField result object :",
                                             true,
                                             nullptr);
        }
        return ret;
    }
    return orig_GetObjectField(env, argObj, jfieldId);
}

//jboolean    (*GetBooleanField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jboolean, GetBooleanField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetBooleanField")
        jboolean ret = orig_GetBooleanField(env, argObj, jfieldId);
        //LOG(INFO) << "GetBooleanField result jboolean : " << (ret == JNI_TRUE?"true":"false");
        //os << "GetBooleanField result jboolean :  " << (ret == JNI_TRUE?"true":"false") << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetBooleanField result jboolean : ").append(
                (ret == JNI_TRUE ? "true" : "false")).append("\n"));
        return ret;
    }
    return orig_GetBooleanField(env, argObj, jfieldId);
}
//jbyte       (*GetByteField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jbyte, GetByteField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetByteField")
        jbyte ret = orig_GetByteField(env, argObj, jfieldId);
        //LOG(INFO) << "GetByteField result jbyte : " << ret;
        //            os << "GetByteField result jbyte :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetByteField result jbyte : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetByteField(env, argObj, jfieldId);
}
//jchar       (*GetCharField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jchar, GetCharField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetCharField")
        jchar ret = orig_GetCharField(env, argObj, jfieldId);
        //LOG(INFO) << "GetCharField result jbyte : " << ret;
        //os << "GetCharField result jbyte :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetCharField result jbyte : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetCharField(env, argObj, jfieldId);
}
//jshort      (*GetShortField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jshort, GetShortField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetShortField")
        jshort ret = orig_GetShortField(env, argObj, jfieldId);
        //LOG(INFO) << "GetShortField result jshort : " << ret;
        //os << "GetShortField result jshort :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetShortField result jshort : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetShortField(env, argObj, jfieldId);
}
//jint        (*GetIntField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jint, GetIntField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetIntField")
        jint ret = orig_GetIntField(env, argObj, jfieldId);
        //LOG(INFO) << "GetIntField result jint : " << ret;
        //os << "GetIntField result jint :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetIntField result jint : ").append(
                to_string(ret)).append("\n"));
        return ret;
    }
    return orig_GetIntField(env, argObj, jfieldId);
}
//jlong       (*GetLongField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jlong, GetLongField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetLongField")
        jlong ret = orig_GetLongField(env, argObj, jfieldId);
        //LOG(INFO) << "GetLongField result jlong : " << ret;
        //os << "GetLongField result jlong :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetLongField result jlong : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetLongField(env, argObj, jfieldId);
}
//jfloat      (*GetFloatField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jfloat, GetFloatField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetFloatField")
        jfloat ret = orig_GetFloatField(env, argObj, jfieldId);
        //LOG(INFO) << "GetFloatField result jfloat : " << ret;
        //os << "GetFloatField result jfloat :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetFloatField result jfloat : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetFloatField(env, argObj, jfieldId);
}
//jdouble     (*GetDoubleField)(JNIEnv*, jobject, jfieldID);
JNITRACE_HOOK_DEF(jdouble, GetDoubleField, JNIEnv *env, [[maybe_unused]] jobject argObj,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        //GET_JOBJECT_INFO(env, argObj,"GetDoubleField")
        jdouble ret = orig_GetDoubleField(env, argObj, jfieldId);
        //LOG(INFO) << "GetDoubleField result jdouble : " << ret;
        //os << "GetDoubleField result jdouble :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetDoubleField result jdouble : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetDoubleField(env, argObj, jfieldId);
}
//--------------------------getStaticField----------------------------
//jobject     (*GetStaticObjectField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jobject, GetStaticObjectField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jobject ret = orig_GetStaticObjectField(env, clazz, jfieldId);
        if (ret != nullptr) {
            GET_ADDRESS
            Jnitrace::getJObjectInfoInternal(env, ret,
                                             string("<") + address +
                                             ">GetStaticObjectField result object :",
                                             true,
                                             nullptr);
        }
        return ret;
    }
    return orig_GetStaticObjectField(env, clazz, jfieldId);
}

//jboolean    (*GetStaticBooleanField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jboolean, GetStaticBooleanField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jboolean ret = orig_GetStaticBooleanField(env, clazz, jfieldId);
        //LOG(INFO) << "GetStaticBooleanField result jboolean : " << (ret == JNI_TRUE?"true":"false");
        //os << "GetStaticBooleanField result jboolean :  " << (ret == JNI_TRUE?"true":"false") << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetStaticBooleanField result jboolean : ").append(
                (ret == JNI_TRUE ? "true" : "false")).append("\n"));
        return ret;
    }
    return orig_GetStaticBooleanField(env, clazz, jfieldId);
}
//jbyte       (*GetStaticByteField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jbyte, GetStaticByteField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jbyte ret = orig_GetStaticByteField(env, clazz, jfieldId);
        //LOG(INFO) << "GetStaticByteField result jbyte : " << ret;
        //            os << "GetStaticByteField result jbyte :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetStaticByteField result jbyte : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetStaticByteField(env, clazz, jfieldId);
}
//jchar       (*GetStaticCharField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jchar, GetStaticCharField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jchar ret = orig_GetStaticCharField(env, clazz, jfieldId);
        //LOG(INFO) << "GetStaticCharField result jchar : " << ret;
        //os << "GetStaticCharField result jchar :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetStaticCharField result jchar : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetStaticCharField(env, clazz, jfieldId);
}
//jshort      (*GetStaticShortField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jshort, GetStaticShortField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jshort ret = orig_GetStaticShortField(env, clazz, jfieldId);
        //LOG(INFO) << "GetStaticShortField result jshort : " << ret;
        //os << "GetStaticShortField result jshort :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetStaticShortField result jshort : ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetStaticShortField(env, clazz, jfieldId);
}
//jint        (*GetStaticIntField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jint, GetStaticIntField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jint ret = orig_GetStaticIntField(env, clazz, jfieldId);
        //LOG(INFO) << "GetStaticIntField result jint : " << ret;
        //            os << "GetStaticIntField result jint :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetStaticIntField result jint :  ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetStaticIntField(env, clazz, jfieldId);
}
//jlong       (*GetStaticLongField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jlong, GetStaticLongField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jlong ret = orig_GetStaticLongField(env, clazz, jfieldId);
        //LOG(INFO) << "GetStaticLongField result jlong : " << ret;
        //os << "GetStaticLongField result jlong :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetStaticLongField result jlong :  ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetStaticLongField(env, clazz, jfieldId);
}
//jfloat      (*GetStaticFloatField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jfloat, GetStaticFloatField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jfloat ret = orig_GetStaticFloatField(env, clazz, jfieldId);
        //LOG(INFO) << "GetStaticFloatField result jfloat : " << ret;
        //os << "GetStaticFloatField result jfloat :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetStaticFloatField result jfloat :  ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetStaticFloatField(env, clazz, jfieldId);
}
//jdouble     (*GetStaticDoubleField)(JNIEnv*, jclass, jfieldID);
JNITRACE_HOOK_DEF(jdouble, GetStaticDoubleField, JNIEnv *env, jclass clazz,
                  [[maybe_unused]] jfieldID jfieldId) {
    DL_INFO
    IS_MATCH
        jdouble ret = orig_GetStaticDoubleField(env, clazz, jfieldId);
        //LOG(INFO) << "GetStaticDoubleField result jdouble : " << ret;
        //os << "GetStaticDoubleField result jdouble :  " << ret << "\n";;
        GET_ADDRESS
        WRITE(string(string("<") + address + ">GetStaticDoubleField result jdouble :  ").append(
                to_string(ret)).append(
                "\n"));
        return ret;
    }
    return orig_GetStaticDoubleField(env, clazz, jfieldId);
}
//jclass DefineClass(const char *name, jobject loader, const jbyte* buf,jsize bufLen)
JNITRACE_HOOK_DEF(jclass, DefineClass, JNIEnv *env, const char *name, jobject loader,
                  const jbyte *buf, jsize bufLen) {
    DL_INFO
    IS_MATCH
        auto ret = orig_DefineClass(env, name, loader, buf, bufLen);
        if (ret != nullptr) {
            //LOG(INFO) << "GetStaticDoubleField result jdouble : " << ret;
            //os << "GetStaticDoubleField result jdouble :  " << ret << "\n";;
            GET_ADDRESS
            WRITE(string(string("<") + address + ">DefineClass   ").append(name).append("\n"));
        }
        return ret;
    }
    return orig_DefineClass(env, name, loader, buf, bufLen);
}


void init_jnitracer(JNIEnv *env) {

    HOOK_JNITRACE(env, CallObjectMethodV)
    HOOK_JNITRACE(env, CallBooleanMethodV)
    HOOK_JNITRACE(env, CallByteMethodV)
    HOOK_JNITRACE(env, CallCharMethodV)
    HOOK_JNITRACE(env, CallShortMethodV)
    HOOK_JNITRACE(env, CallIntMethodV)
    HOOK_JNITRACE(env, CallLongMethodV)
    HOOK_JNITRACE(env, CallFloatMethodV)
    HOOK_JNITRACE(env, CallDoubleMethodV)
    HOOK_JNITRACE(env, CallVoidMethodV)

    HOOK_JNITRACE(env, CallStaticObjectMethodV)
    HOOK_JNITRACE(env, CallStaticBooleanMethodV)
    HOOK_JNITRACE(env, CallStaticByteMethodV)
    HOOK_JNITRACE(env, CallStaticCharMethodV)
    HOOK_JNITRACE(env, CallStaticShortMethodV)
    HOOK_JNITRACE(env, CallStaticIntMethodV)
    HOOK_JNITRACE(env, CallStaticLongMethodV)
    HOOK_JNITRACE(env, CallStaticFloatMethodV)
    HOOK_JNITRACE(env, CallStaticDoubleMethodV)
    HOOK_JNITRACE(env, CallStaticVoidMethodV)


    HOOK_JNITRACE(env, GetObjectField)
    HOOK_JNITRACE(env, GetBooleanField)
    HOOK_JNITRACE(env, GetByteField)
    HOOK_JNITRACE(env, GetCharField)
    HOOK_JNITRACE(env, GetShortField)
    HOOK_JNITRACE(env, GetIntField)
    HOOK_JNITRACE(env, GetLongField)
    HOOK_JNITRACE(env, GetFloatField)
    HOOK_JNITRACE(env, GetDoubleField)

    HOOK_JNITRACE(env, GetStaticObjectField)
    HOOK_JNITRACE(env, GetStaticBooleanField)
    HOOK_JNITRACE(env, GetStaticByteField)
    HOOK_JNITRACE(env, GetStaticCharField)
    HOOK_JNITRACE(env, GetStaticShortField)
    HOOK_JNITRACE(env, GetStaticIntField)
    HOOK_JNITRACE(env, GetStaticLongField)
    HOOK_JNITRACE(env, GetStaticFloatField)
    HOOK_JNITRACE(env, GetStaticDoubleField)

    //常用的字符串操作函数
    HOOK_JNITRACE(env, NewStringUTF)
    HOOK_JNITRACE(env, GetStringUTFChars)

    HOOK_JNITRACE(env, FindClass)
    HOOK_JNITRACE(env, DefineClass)
    HOOK_JNITRACE(env, ToReflectedMethod)
    HOOK_JNITRACE(env, FromReflectedMethod)


    HOOK_JNITRACE(env, GetFieldID)
    HOOK_JNITRACE(env, GetStaticFieldID)

    HOOK_JNITRACE(env, NewObjectV)

    //将C数组中的数据复制到Java字节数组
    HOOK_JNITRACE(env, SetByteArrayRegion)
    HOOK_JNITRACE(env, GetByteArrayRegion)

    //从Java字符串中获取指定范围的字符
    HOOK_JNITRACE(env, GetStringRegion)
    HOOK_JNITRACE(env, GetStringUTFRegion)


    LOG(INFO) << ">>>>>>>>> Jnitrace hook success! ";
}


void Jnitrace::init(JNIEnv *env,
                             bool hookAll,
                             const std::list<string> &forbid_list,
                             const std::list<string> &filter_list,
                             std::ofstream *os) {
    isHookAll = hookAll;
    LOGE("start jni trace is hook all %s", isHookAll ? "true" : "false")
    for (const std::string &str: forbid_list) {
        LOGE("start jni trace forbid_list %s", str.c_str())
    }

    //copy orig list
    forbidSoList = std::list<string>(forbid_list);
    filterSoList = std::list<string>(filter_list);
    if (os != nullptr) {
        isSave = true;
        traceOs = os;
    }
    init_jnitracer(env);
}

[[maybe_unused]]
void Jnitrace::stop() {
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

void Jnitrace::setAuxiliaryClazz(jclass clazz) {
    auxiliary_clazz = clazz;
}


