cmake_minimum_required(VERSION 3.4.1)
project(nativeLib)
ENABLE_LANGUAGE(ASM)
message(STATUS "nativeLib start building ")

set(NATIVE_LIB_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_library(nativeLib INTERFACE)



target_include_directories(
        nativeLib
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/
)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/
)

add_subdirectory(dlfc)
add_subdirectory(libpath)
add_subdirectory(adapter)
add_subdirectory(utils)
add_subdirectory(hook)
add_subdirectory(xunwind)
add_subdirectory(linker)
add_subdirectory(findsyscall)
add_subdirectory(hidemaps)
add_subdirectory(proc_maps_parser)
add_subdirectory(antiJit)
add_subdirectory(tracer)


if (ZHENXI_BUILD_TYPE STREQUAL "Release")
    #Hikari
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-bcfobf") #启用伪控制流
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -bcf_prob=20") #每个基本块有20%的概率进行BCF混淆
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -bcf_loop=5") #每个函数上进行三次BCF混淆
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-funcwra") #启用函数封装
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-subobf") #启用指令替换
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-strcry") #启用字符串加密
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-splitobf") #启用基本块分割
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-acdobf") #启用反class-dump
    #耗时字段,这个字段加上去编译会很长时间
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-cffobf") #启用控制流平坦化
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -fw_prob=50") #有50%的概率应用函数包装技术到每个函数上
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -fw_times=3") #每个函数将应用3次函数包装技术。
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-indibran") #反汇编程序无法从静态分析中预测完整的控制流程


    #ollvm
    #    #即控制流平坦化（Control Flow Flattening）
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -fla")
    #    #即基本块分割（Basic Block Splitting） 基本块分割（Split）的数量为3
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -split -mllvm -split_num=3")
    #    #SUB即逻辑指令替换（Substitution） 指令替换的循环次数为3
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -sub -mllvm -sub_loop=3")
    #----------------------------------------------------------------------------------
    #    #BCF即假代码注入（Bogus Control Flow）假代码注入（BCF）的概率为40%(编译耗时,体积膨胀)
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -bcf -mllvm -bcf_prob=40")
    #    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -bcf")
endif ()
#ollvm raw_syscall
add_subdirectory(raw_syscall)

target_link_libraries(
        nativeLib
        INTERFACE

        dlfc libpath adapter utils runtimehook raw_syscall xunwind
        linkerHandler findsyscall mapshidepath proc_maps_parser antijit
        tracer
)
