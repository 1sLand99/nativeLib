cmake_minimum_required(VERSION 3.4.1)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wwritable-strings")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wwritable-strings")


add_library(
        tracer

        STATIC

        HookClassUtils.cpp
        invokePrintf.cpp
        JnitraceForC.cpp
        StringHandler.cpp
        TracerBase.cpp
)

target_link_libraries(
        tracer

        ${log-lib}
)

