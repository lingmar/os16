#include "system_calls.h"

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

void close_exit_on_failure(int fd) {
    Sys_call_1_exit_on_failure(close, fd);
}

void pipe_exit_on_failure(int fildes[2]) {
    Sys_call_1_exit_on_failure(pipe, fildes);
}

void kill_exit_on_failure(pid_t pid, int sig) {
    Sys_call_2_exit_on_failure(kill, pid, sig);
}

void dup2_exit_on_failure(int fd1, int fd2) {
    Sys_call_2_exit_on_failure(dup2, fd1, fd2);
}

void write_exit_on_failure(int fd, const void *buf, size_t nbyte) {
    Sys_call_3_exit_on_failure(write, fd, buf, nbyte);
}

void read_exit_on_failure(int fildes, void *buf, size_t nbyte) {
    Sys_call_3_exit_on_failure(read, fildes, buf, nbyte);
}

void signal_exit_on_failure(int sig, sig_t func) {
    if (signal(sig, func) == SIG_ERR) {
        perror("Error using signal(). Exiting.");       
        exit(EXIT_FAILURE);
    }                                                   
}
