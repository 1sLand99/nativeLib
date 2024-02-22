//
// Created by Zhenxi on 2024/2/22.
//

#include "raw_syscall.h"

INLINE long raw_syscall(long __number, ...) {
    long result;
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
    return result;
}
