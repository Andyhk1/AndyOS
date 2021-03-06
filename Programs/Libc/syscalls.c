#include "syscall_list.h"
#include <dirent.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

int set_err(int ret)
{
    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}

//

int sched_yield()
{
    return 0;
}

int access(const char *pathname, int mode)
{
    struct stat st;

    if (stat(pathname, &st) == 0)
        return 0;

    errno = EACCES;
    return -1;
}

mode_t umask(mode_t mask)
{
    return 0;
}

int lstat(const char *path, struct stat *buf)
{
    return stat(path, buf); // TODO
}

uid_t getuid()
{
    return 0;
}

uid_t getgid()
{
    return 0;
}

uid_t geteuid()
{
    return 0;
}

uid_t getegid()
{
    return 0;
}

struct group *getgrgid(gid_t gid)
{
    return 0;
}

pid_t tcgetpgrp(int fd)
{
    int p;

    if (ioctl(fd, TIOCGPGRP, &p) < 0)
        return (pid_t)-1;

    return (pid_t)p;
}

int tcsetpgrp(int fd, pid_t pgrp)
{
    int p = (int)pgrp;
    return ioctl(fd, TIOCSPGRP, &p);
}

int tcgetattr(int fd, struct termios *termios_p)
{
    return ioctl(fd, TCGETS, termios_p);
}

int tcsetattr(int fd, int optional_actions, const struct termios *termios_p)
{
    int cmd;

    switch (optional_actions) {
    case TCSANOW:
        cmd = TCSETS;
        break;
    case TCSADRAIN:
        cmd = TCSETSW;
        break;
    case TCSAFLUSH:
        cmd = TCSETSF;
        break;
    default:
        errno = EINVAL;
        return -1;
    }

    return ioctl(fd, cmd, termios_p);
}

int tcdrain(int fd)
{
    return ioctl(fd, TCSBRK, 1);
}

int tcflow(int fd, int action)
{
    return ioctl(fd, TCXONC, action);
}

int tcflush(int fd, int queue_selector)
{
    return ioctl(fd, TCFLSH, queue_selector);
}

int tcsendbreak(int fd, int duration)
{
    if (duration <= 0)
        return ioctl(fd, TCSBRK, 0);

    errno = (EINVAL);
    return -1;
}

pid_t tcgetsid(int fd)
{
    return 0;
}

int tcsetsid(int fd, pid_t pid)
{
    return 0;
}

int getpriority(int which, id_t who)
{
    return 0;
}

int setpriority(int which, id_t who, int prio)
{
    return 0;
}

int getrlimit(int resource, struct rlimit *rlim)
{
    return 0;
}

int setrlimit(int resource, const struct rlimit *rlim)
{
    return 0;
}

int getrusage(int who, struct rusage *usage)
{
    return 0;
}

int issetugid()
{
    return 0;
}

long sysconf(int name)
{
    return -1;
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz)
{
    return 0;
}

int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    fds->revents = fds->events;
    return 1;
}

int clock_settime(clockid_t clock_id, const struct timespec *tp)
{
    return 0;
}

int getgrouplist(const char *user, gid_t group, gid_t *groups, int *ngroups)
{
    return 0;
}

int truncate(const char *path, off_t length)
{
    return 0;
}

int ftruncate(int fd, off_t length)
{
    return 0;
}

int getpagesize()
{
    return 4096;
}

////

int *__errno()
{
    return &_REENT->_errno;
}

void _exit(int status)
{
    syscall1(SYSCALL_EXIT, status);
}

int chdir(const char *path)
{
    return set_err(syscall1(SYSCALL_CHDIR, path));
}

int _close(int fd)
{
    return set_err(syscall1(SYSCALL_CLOSE, fd));
}

int dup(int oldfd)
{
    return set_err(syscall1(SYSCALL_DUP, oldfd));
}

int dup2(int oldfd, int newfd)
{
    return set_err(syscall2(SYSCALL_DUP2, oldfd, newfd));
}

int _execve(const char *path, char *const argv[], char *const envp[])
{
    return set_err(syscall3(SYSCALL_EXECVE, (size_t)path, (size_t)argv, (size_t)envp));
}

int fchdir(int fd)
{
    return set_err(syscall1(SYSCALL_FCHDIR, fd));
}

int _fcntl(int fd, int cmd, ...)
{
    int arg = 0;
    if ((cmd == F_DUPFD) || (cmd == F_SETFD) || (cmd == F_SETFL)) {
        va_list args;
        va_start(args, cmd);
        arg = va_arg(args, int);
        va_end(args);
    }

    return set_err(syscall3(SYSCALL_FCNTL, fd, cmd, arg));
}

pid_t _fork()
{
    return set_err(syscall0(SYSCALL_FORK));
}

pid_t vfork()
{
    return fork();
}

int _fstat(int fd, struct stat *st)
{
    return set_err(syscall2(SYSCALL_FSTAT, fd, st));
}

/*int getdents(unsigned int fd, struct dirent *dirp, unsigned int count)
{
    return syscall3(SYSCALL_GETDENTS, fd, dirp, count);
}*/

int getdents(int fd, void *dp, int count)
{
    return set_err(syscall3(SYSCALL_GETDENTS, fd, dp, count));
}

int _gettimeofday(struct timeval *tv, void *tz)
{
    return set_err(syscall2(SYSCALL_GETTIMEOFDAY, tv, tz));
}

int ioctl(int fd, int request, ...)
{
    // TODO
    int arg = 0;
    va_list args;
    va_start(args, request);
    arg = va_arg(args, int);
    va_end(args);

    return set_err(syscall3(SYSCALL_IOCTL, fd, request, arg));
}

int _isatty(int fd)
{
    struct termios term;
    return tcgetattr(fd, &term) == 0;
}

int _kill(int pid, int sig)
{
    return set_err(syscall2(SYSCALL_KILL, pid, sig));
}

int _link(char *path1, char *path2)
{
    return set_err(syscall2(SYSCALL_LINK, path1, path2));
}

off_t _lseek(int fd, off_t offset, int whence)
{
    return set_err(syscall3(SYSCALL_LSEEK, fd, offset, whence));
}

int _open(const char *name, int flags, ...)
{
    mode_t mode = 0;

    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = va_arg(args, mode_t);
        va_end(args);
    }

    return set_err(syscall3(SYSCALL_OPEN, name, flags, mode));
}

int pipe(int pipefd[2])
{
    return set_err(syscall1(SYSCALL_PIPE, pipefd));
}

ssize_t _read(int fd, char *buf, size_t size)
{
    return set_err(syscall3(SYSCALL_READ, fd, buf, size));
}

caddr_t _sbrk(intptr_t increment)
{
    return (caddr_t)set_err(syscall1(SYSCALL_SBRK, increment));
}

sig_t signal(int signum, sig_t handler)
{
    volatile struct sigaction act;
    volatile struct sigaction old;

    act.sa_handler = handler;
    act.sa_mask = 0;
    act.sa_flags = 0;

    if (sigaction(signum, &act, &old) == -1)
        return SIG_ERR;

    return (sig_t)old.sa_handler;
}

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    return set_err(syscall3(SYSCALL_SIGACTION, signum, act, oldact));
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
    return set_err(syscall3(SYSCALL_SIGPROCMASK, how, set, oset));
}

int sigreturn()
{
    return set_err(syscall0(SYSCALL_SIGRETURN));
}

int sigsuspend(const sigset_t *mask)
{
    return set_err(syscall1(SYSCALL_SIGSUSPEND, mask));
}

int _stat(const char *file, struct stat *st)
{
    return set_err(syscall2(SYSCALL_STAT, file, st));
}

clock_t _times(struct tms *buf)
{
    return set_err(syscall1(SYSCALL_TIMES, buf));
}

int uname(struct utsname *name)
{
    return syscall1(SYSCALL_UNAME, name);
}

int _unlink(char *name)
{
    return set_err(syscall1(SYSCALL_UNLINK, name));
}

int usleep(useconds_t usec)
{
    return set_err(syscall1(SYSCALL_SLEEP, usec));
}

int _wait(int *status)
{
    return waitpid(-1, status, 0);
}

pid_t wait3(int *status, int options, struct rusage *rusage)
{
    return waitpid(-1, status, options);
}

pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage)
{
    return waitpid(pid, status, options);
}

int waitpid(pid_t pid, int *status, int options)
{
    return set_err(syscall3(SYSCALL_WAITPID, pid, status, options));
}

/*int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options)
{
    return set_err(syscall4(SYSCALL_WAITID, idtype, id, infop, options));
}*/

ssize_t _write(int fd, const char *buf, size_t size)
{
    return (ssize_t)set_err(syscall3(SYSCALL_WRITE, fd, buf, size));
}

//

int getgroups(int size, gid_t list[])
{
    return set_err(syscall2(SYSCALL_GETGROUPS, size, list));
}

pid_t getpgid(pid_t pid)
{
    return set_err(syscall1(SYSCALL_GETPGID, pid));
}

pid_t getpgrp()
{
    return getpgid(0);
}

pid_t _getpid()
{
    return set_err(syscall0(SYSCALL_GETPID));
}

pid_t getppid()
{
    return set_err(syscall0(SYSCALL_GETPPID));
}

int setpgid(pid_t pid, pid_t pgid)
{
    return set_err(syscall2(SYSCALL_SETPGID, pid, pgid));
}

//

int socket(int domain, int type, int protocol)
{
    return set_err(syscall3(SYSCALL_SOCKET, domain, type, protocol));
}

int accept(int fd, struct sockaddr *addr, socklen_t addrlen, int flags)
{
    return set_err(syscall4(SYSCALL_ACCEPT, fd, addr, addrlen, flags));
}

int bind(int fd, const struct sockaddr *addr, socklen_t addrlen)
{
    return set_err(syscall3(SYSCALL_BIND, fd, addr, addrlen));
}

int connect(int fd, const struct sockaddr *addr, socklen_t addrlen)
{
    return set_err(syscall3(SYSCALL_CONNECT, fd, addr, addrlen));
}

int listen(int fd, int backlog)
{
    return set_err(syscall2(SYSCALL_LISTEN, fd, backlog));
}

int recv(int fd, void *buf, size_t len, int flags)
{
    return set_err(syscall4(SYSCALL_RECV, fd, buf, len, flags));
}

int recvfrom(int fd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t addrlen)
{
    return set_err(syscall6(SYSCALL_RECVFROM, fd, buf, len, flags, src_addr, addrlen));
}

int recvmsg(int fd, struct msghdr *msg, int flags)
{
    return set_err(syscall3(SYSCALL_RECVMSG, fd, msg, flags));
}

int send(int fd, const void *buf, size_t len, int flags)
{
    return set_err(syscall4(SYSCALL_SEND, fd, buf, len, flags));
}

int sendmsg(int fd, const struct msghdr *msg, int flags)
{
    return set_err(syscall3(SYSCALL_SENDMSG, fd, msg, flags));
}

int sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr,
           socklen_t addrlen)
{
    return set_err(syscall6(SYSCALL_SENDTO, fd, buf, len, flags, dest_addr, addrlen));
}

int shutdown(int fd, int how)
{
    return set_err(syscall2(SYSCALL_SHUTDOWN, fd, how));
}

unsigned int sleep(unsigned int seconds)
{
    return usleep(seconds * 1000000);
}
