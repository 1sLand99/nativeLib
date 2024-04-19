cmake_minimum_required(VERSION 3.4.1)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -std=c++20")

#添加内联支持
enable_language(C ASM)







#这个so主要包含各种指纹修改相关函数
add_library(
        netlink

        STATIC

        getNetLinkerMac.cpp

)

target_link_libraries(
        netlink
        ${log-lib}
)









