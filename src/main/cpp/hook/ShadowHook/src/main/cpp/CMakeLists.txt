cmake_minimum_required(VERSION 3.22.1)
project(shadowhook)

if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ARCH "arm64")
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(ARCH "arm")
endif()

set(TARGET "shadowhook")
file(GLOB SRC *.cpp arch/${ARCH}/*.cpp common/*.cpp third_party/*/*.cpp)
add_library(${TARGET} STATIC ${SRC})

#target_compile_features(${TARGET} PUBLIC c_std_17)
#target_compile_options(${TARGET} PUBLIC -std=c17 -Weverything -Werror)

#target_include_directories(${TARGET} PUBLIC . include arch/${ARCH} common third_party/xdl third_party/bsd third_party/lss)
include_directories(
        .
        include
        arch/${ARCH}
        common
        third_party/bsd
        third_party/lss
)

target_link_libraries(${TARGET} log)

#if(USEASAN)
#    target_compile_options(${TARGET} PUBLIC -fsanitize=address -fno-omit-frame-pointer)
#    target_link_options(${TARGET} PUBLIC -fsanitize=address)
#else()
#    target_compile_options(${TARGET} PUBLIC -Oz -ffunction-sections -fdata-sections)
#    target_link_options(${TARGET} PUBLIC -Oz -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/shadowhook.map.txt)
#endif()
