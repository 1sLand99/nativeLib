

#ifndef QCONTAINER_PRO_APPUTILS_H
#define QCONTAINER_PRO_APPUTILS_H

#include "AllInclude.h"


typedef size_t Addr;


class Jnitrace {
public:
    /**
     * start jni trace
     *
     * @param env  jniEnv
     * @param hookAll  lister all so jni
     * @param forbid_list  not lister so list
     * @param filter_list  lister so list
     * @param os using the list to save the entire collection,
     *           the jnitrace only handles the so name inside the list 。
     *           No file save occurs if os == nullptr
     */
    [[maybe_unused]] static void startjnitrace(JNIEnv *env,
                              bool hookAll,
                              const std::list<string> &forbid_list,
                              const std::list<string> &filter_list,
                              std::ofstream *os);

    /**
     * stop jni trace
     */
    [[maybe_unused]]
    static void stopjnitrace();

    static void setAuxiliaryClazz(jclass clazz);

    static bool isLister(int dladd_ret, Dl_info *info) ;
    static void write(const std::string &msg) ;
    static void init(JNIEnv *env);
    static void write(const std::string &msg, [[maybe_unused]] bool isApart);

    static void
    getArgsInfo(JNIEnv *env, jobject obj, jmethodID jmethodId, va_list args, bool isStatic);

    static void getJObjectInfo(JNIEnv *env, jobject obj, const string &methodname);

    static const string getJObjectToString(JNIEnv *env, jobject obj,const char * classInfo);

    static void
    getJObjectInfoInternal(JNIEnv *env, jobject obj, string message, bool isPrintClassinfo,
                           const char *classinfo);

    static string getJObjectClassInfo(JNIEnv *env, jobject obj);

private:

    static bool isHookAll;
    static std::ofstream *jnitraceOs;
    static std::list<string> filterSoList;
    static std::list<string> forbidSoList;
    static bool isSave;
    static string match_so_name;
    //static std::mutex supernode_ids_mux_;
    /**
     * 在打印枚举的时候发现可能会被当成object去打印
     * 导致报错
     * JNI DETECTED ERROR IN APPLICATION: use of invalid jobject
     * 尝试在Java去打印
     */
    static jclass AuxiliaryClazz;
    /**
      * 是否监听第二级返回结果。
    */
    static bool isHookSecondRet;
    static jmethodID object_method_id_toString;
    //static jmethodID enum_method_id_toString;
    static jmethodID AuxiliaryClazz_method_id_toString ;

};


#endif //QCONTAINER_PRO_APPUTILS_H
