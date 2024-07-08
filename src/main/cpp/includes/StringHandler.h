//
// Created by zhenxi on 2022/1/21.
//

#ifndef QCONTAINER_PRO_STRINGHANDLER_H
#define QCONTAINER_PRO_STRINGHANDLER_H

#include <list>

#include "stringUtils.h"
#include "libpath.h"
#include "TracerBase.h"


class StringHandler :TracerBase{
public:
    void init(JNIEnv *env,
              bool hookAll,
              const std::list<std::string> &forbid_list,
              const std::list<std::string> &filter_list,
              std::ofstream *os) override;
    void stop() override;

};


#endif //QCONTAINER_PRO_STRINGHANDLER_H
