//
// Created by Zhenxi on 2022/8/22.
//

#ifndef HUNTER_RAW_SYSCALL_H
#define HUNTER_RAW_SYSCALL_H



#define INLINE __always_inline

#include <asm/unistd.h>
#include <sys/syscall.h>


#define syscall_mod_mem 1
#define syscall_mod_inline 2

struct syscall_mem_ptr_info{
    unsigned int size ;
    void* mem_ptr;
};

long raw_syscall(long __number, ...);
syscall_mem_ptr_info getSyscallMemPtr();
inline void setRawSyscallMod(int mod);

//void init_mem_raw_syscall();

#endif //HUNTER_RAW_SYSCALL_H
