#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>

#define Sys_call_1_exit_on_failure(sys_call, arg)                       \
    if (sys_call(arg) < 0) {                                            \
        fprintf(stderr, "Error using %s(). Exiting.", #sys_call);       \
        exit(EXIT_FAILURE);                                             \
    }

#define Sys_call_2_exit_on_failure(sys_call, arg1, arg2)                \
    if (sys_call(arg1, arg2) < 0) {                                     \
        fprintf(stderr, "Error using %s(). Exiting.", #sys_call);       \
        exit(EXIT_FAILURE);                                             \
    }

#define Sys_call_3_exit_on_failure(sys_call, arg1, arg2, arg3)          \
    if (sys_call(arg1, arg2, arg3) < 0) {                               \
        fprintf(stderr, "Error using %s(). Exiting.", #sys_call);       \
        exit(EXIT_FAILURE);                                             \
    }

void close_exit_on_failure(int fd);

void pipe_exit_on_failure(int fildes[2]);

void kill_exit_on_failure(pid_t pid, int sig);

void dup2_exit_on_failure(int fd1, int fd2);

void write_exit_on_failure(int fd, const void *buf, size_t nbyte);

void read_exit_on_failure(int fildes, void *buf, size_t nbyte);

void signal_exit_on_failure(int sig, sig_t func);
