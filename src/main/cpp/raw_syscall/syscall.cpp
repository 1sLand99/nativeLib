//
// Created by Zhenxi on 2024/2/22.
//

#include "raw_syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <syscall.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#include "ZhenxiLog.h"
#include "logging.h"
#include "xdl.h"

// 定义一个函数指针类型，以便于执行动态生成的代码
typedef long (*syscall_func)(long number, ...);

#if defined(__aarch64__)
const unsigned char syscall_code[] = {
        0xe8, 0x03, 0x00, 0xaa, 0xe0, 0x03, 0x01, 0xaa, 0xe1, 0x03, 0x02, 0xaa,
        0xe2, 0x03, 0x03, 0xaa, 0xe3, 0x03, 0x04, 0xaa, 0xe4, 0x03, 0x05, 0xaa,
        0xe5, 0x03, 0x06, 0xaa, 0x01, 0x00, 0x00, 0xd4, 0x1f, 0x04, 0x40, 0xb1,
        0x00, 0x94, 0x80, 0xda, 0x48, 0xbb, 0x29, 0x54, 0xc0, 0x03, 0x5f, 0xd6
};

#else
const unsigned char syscall_code[] = {
    0x0D, 0xC0, 0xA0, 0xE1, 0xF0, 0x00, 0x2D, 0xE9,
    0x00, 0x70, 0xA0, 0xE1, 0x01, 0x00, 0xA0, 0xE1,
    0x02, 0x10, 0xA0, 0xE1, 0x03, 0x20, 0xA0, 0xE1,
    0x78, 0x00, 0x9C, 0xE8, 0x00, 0x00, 0x00, 0xEF,
    0xF0, 0x00, 0xBD, 0xE8, 0x01, 0x0A, 0x70, 0xE3,
    0x1E, 0xFF, 0x2F, 0x91, 0x00, 0x00, 0x60, 0xE2,
    0x63, 0x72, 0x01, 0xEA
};

#endif

void print_hex(void* ptr, size_t length) {
    unsigned char* bytes = (unsigned char*)ptr;
    char* output = (char*)malloc(length * 3 + 1);
    if (!output) {
        // 内存分配失败
        LOG(ERROR) << "Memory allocation failed\n";
        return;
    }
    char* current = output;
    for (size_t i = 0; i < length; ++i) {
        current += sprintf(current, "%02x ", bytes[i]);
    }
    LOG(ERROR)<<length<<" code -> " << output;
    free(output);
}


void* syscall_mem = nullptr;

INLINE long raw_syscall(long __number, ...) {

//    void* handle = xdl_open("libc.so", XDL_ALWAYS_FORCE_LOAD);
//    size_t size =-1;
//    void* syscall_addr = xdl_sym(handle,"syscall",&size);
//    print_hex(syscall_addr,size);
//    exit(0);


    long result ;
#if defined(__aarch64__)
    __asm__ (
            "MOV X8, X0\n\t"
            "MOV X0, X1\n\t"
            "MOV X1, X2\n\t"
            "MOV X2, X3\n\t"
            "MOV X3, X4\n\t"
            "MOV X4, X5\n\t"
            "MOV X5, X6\n\t"
            "SVC 0\n\t"
            "MOV %0, X0\n\t"
            : "=r" (result)
            :
            : "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x8" // 破坏列表
            );
#else
    __asm__ (
        "MOV R12, SP\n\t"
        "STMFD SP!, {R4-R7}\n\t"
        "MOV R7, R0\n\t"
        "MOV R0, R1\n\t"
        "MOV R1, R2\n\t"
        "MOV R2, R3\n\t"
        "LDMIA R12, {R3-R6}\n\t"
        "SVC 0\n\t"
        "LDMFD SP!, {R4-R7}\n\t"
        "MOV %0, R0\n\t"
        : "=r" (result)
        :
        : "r0", "r1", "r2", "r3", "r4", "r7", "r12"
    );
#endif

//    if(syscall_mem == nullptr){
//        syscall_mem = mmap(nullptr, sizeof(syscall_code),
//                        PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
//    }
//    if (syscall_mem == MAP_FAILED) {
//        LOG(ERROR)<<"raw_syscall mmap error "<< strerror(errno);
//        return -1;
//    }
//    std::memset(syscall_mem, 0, sizeof(syscall_code));
//    std::memcpy(syscall_mem, syscall_code, sizeof(syscall_code));
//    syscall_func func = (syscall_func)syscall_mem;
//    va_list args;
//    va_start(args, __number);
//    long result = func(__number,args);
//    munmap(syscall_mem, sizeof(syscall_code));
    return result;
}
