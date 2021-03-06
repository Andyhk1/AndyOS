#pragma once

#define SYSCALL_IRQ  0x80
#define MAX_SYSCALLS 1024

#define syscall0(id)                   syscall6(id, 0, 0, 0, 0, 0, 0)
#define syscall1(id, arg0)             syscall6(id, (int)arg0, 0, 0, 0, 0, 0)
#define syscall2(id, arg0, arg1)       syscall6(id, (int)arg0, (int)arg1, 0, 0, 0, 0)
#define syscall3(id, arg0, arg1, arg2) syscall6(id, (int)arg0, (int)arg1, (int)arg2, 0, 0, 0)
#define syscall4(id, arg0, arg1, arg2, arg3) \
    syscall6(id, (int)arg0, (int)arg1, (int)arg2, (int)arg3, 0, 0)
#define syscall5(id, arg0, arg1, arg2, arg3, arg4) \
    syscall6(id, (int)arg0, (int)arg1, (int)arg2, (int)arg3, (int)arg4, 0)

inline int syscall6(int id, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    int ret;

    asm volatile("push %%ebp\n"
                 "mov %2, %%ebp\n"
                 "int %1\n"
                 "pop %%ebp\n"
                 "mov %%eax, %0"
                 : "=m"(ret)
                 : "N"(SYSCALL_IRQ), "g"(arg5), "a"(id), "b"(arg0), "c"(arg1), "d"(arg2), "S"(arg3),
                   "D"(arg4));

    return ret;
}

enum SYSCALLS
{
    SYSCALL_CLOSE,
    SYSCALL_CHDIR,
    SYSCALL_DUP,
    SYSCALL_DUP2,
    SYSCALL_EXECVE,
    SYSCALL_EXIT,
    SYSCALL_FCHDIR,
    SYSCALL_FCNTL,
    SYSCALL_FORK,
    SYSCALL_FSTAT,
    SYSCALL_GETCWD,
    SYSCALL_GETDENTS,
    SYSCALL_GETTIMEOFDAY,
    SYSCALL_IOCTL,
    SYSCALL_KILL,
    SYSCALL_LINK,
    SYSCALL_LSEEK,
    SYSCALL_OPEN,
    SYSCALL_PIPE,
    SYSCALL_READ,
    SYSCALL_SBRK,
    SYSCALL_SIGACTION,
    SYSCALL_SIGPROCMASK,
    SYSCALL_SIGRETURN,
    SYSCALL_SIGSUSPEND,
    SYSCALL_STAT,
    SYSCALL_TIMES,
    SYSCALL_UNAME,
    SYSCALL_UNLINK,
    SYSCALL_WAITID,
    SYSCALL_WAITPID,
    SYSCALL_WRITE,

    SYSCALL_GETGROUPS,
    SYSCALL_GETPGID,
    SYSCALL_GETPID,
    SYSCALL_GETPPID,
    SYSCALL_SETPGID,

    SYSCALL_SOCKET,
    SYSCALL_CONNECT,
    SYSCALL_ACCEPT,
    SYSCALL_BIND,
    SYSCALL_LISTEN,
    SYSCALL_RECV,
    SYSCALL_RECVFROM,
    SYSCALL_RECVMSG,
    SYSCALL_SEND,
    SYSCALL_SENDTO,
    SYSCALL_SENDMSG,
    SYSCALL_SHUTDOWN,
    SYSCALL_SETSOCKETOPT,
    SYSCALL_GETSOCKETOPT,

    SYSCALL_THREAD_CREATE,
    SYSCALL_THREAD_EXIT,

    SYSCALL_GETTIME,
    SYSCALL_DRAW,
    SYSCALL_SLEEP,
    SYSCALL_GET_TICKS,
    SYSCALL_ALLOC_SHARED,
    SYSCALL_READ_FILE,
    SYSCALL_CREATE_PROCESS,
    SYSCALL_DEBUG_RESET,
    SYSCALL_HALT,
    SYSCALL_PRINT,
    SYSCALL_COLOR,
};
