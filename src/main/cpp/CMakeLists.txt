cmake_minimum_required(VERSION 3.4.1)
project(nativeLib)
ENABLE_LANGUAGE(ASM)
message(STATUS "nativeLib start building ")

#basic optimization
if (ZHENXI_BUILD_TYPE STREQUAL "Release")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--gc-sections")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    add_definitions(-DZHENXI_BUILD_TYPE_NOLOG)
endif ()

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
add_subdirectory(raw_syscall)

target_link_libraries(
        nativeLib
        INTERFACE

        dlfc libpath adapter utils runtimehook raw_syscall xunwind
        linkerHandler findsyscall mapshidepath proc_maps_parser antijit
        tracer
)
