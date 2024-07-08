//
// Created by Zhenxi on 2024/7/8.
//

#ifndef RUNTIME_ITRACERBASE_H
#define RUNTIME_ITRACERBASE_H

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


class ITracerBase {
    public:
        virtual ~ITracerBase() = default;
        virtual void init(JNIEnv *env,
                          bool hookAll,
                          const std::list<std::string> &forbid_list,
                          const std::list<std::string> &filter_list,
                          std::ofstream *os) = 0;
        virtual void stop() = 0;
};

#endif //RUNTIME_ITRACERBASE_H
