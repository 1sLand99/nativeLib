cmake_minimum_required(VERSION 3.4.1)
#添加内联支持
enable_language(C ASM)

#------------------shadowhook--------------------------
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/ShadowHook/src/main/cpp/include
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ShadowHook/src/main/cpp)
#------------------dobby--------------------------
#设置路径,引入Dobby
include_directories(
        Dobby/include
        Dobby/source
        Dobby/builtin-plugin
        Dobby/builtin-plugin/AndroidRestriction
        Dobby/builtin-plugin/SymbolResolver
        Dobby/external/logging
)
macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()
# 关闭DobbyDebug日志
SET_OPTION(DOBBY_DEBUG NO)
# 关闭shared库,和我们的so 集成到一起。
SET_OPTION(DOBBY_GENERATE_SHARED OFF)
#是否加载linker加载so的回调
SET_OPTION(Plugin.LinkerLoadCallback OFF)
add_subdirectory(Dobby dobby.build)
#------------------dobby--------------------------




add_library(
        runtimehook

        STATIC

        HookUtils.cpp
)

target_link_libraries(
        runtimehook
        dobby
        shadowhook

        dlfc
        ${log-lib}
)











