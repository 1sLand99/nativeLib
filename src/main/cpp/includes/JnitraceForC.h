

#ifndef QCONTAINER_PRO_APPUTILS_H
#define QCONTAINER_PRO_APPUTILS_H

#include "AllInclude.h"
#include "TracerBase.h"




class Jnitrace : TracerBase{

public:

    void init(JNIEnv *env,
              bool hookAll,
              const std::list<std::string> &forbid_list,
              const std::list<std::string> &filter_list,
              std::ofstream *os) override;
    void stop() override;
    /**
     * 设置toString方法调用的java方法
     */
    void setAuxiliaryClazz(jclass clazz);

    static void
    getArgsInfo(JNIEnv *env, jobject obj, jmethodID jmethodId, va_list args, bool isStatic);

    static void getJObjectInfo(JNIEnv *env, jobject obj, const string &methodname);

    static string getJObjectToString(JNIEnv *env, jobject obj, const char *classInfo);

    static void
    getJObjectInfoInternal(JNIEnv *env, jobject obj, string message, bool isPrintClassinfo,
                           const char *classinfo);

    static string getJObjectClassInfo(JNIEnv *env, jobject obj);

private:
    static jmethodID object_method_id_toString;
    /**
     * 在打印枚举的时候发现可能会被当成object去打印
     * 导致报错
     * JNI DETECTED ERROR IN APPLICATION: use of invalid jobject
     * 尝试在Java去打印。需要实现obj2str方法。
     *"(Ljava/lang/Object;)Ljava/lang/String;");
     */
    static jclass auxiliary_clazz;

    static jmethodID auxiliary_clazz_method_id_toString ;

};


#endif //QCONTAINER_PRO_APPUTILS_H
