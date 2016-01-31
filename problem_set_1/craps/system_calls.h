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

#define Close_exit_on_failure(fd)               \
    Sys_call_1_exit_on_failure(close, fd);      \

#define Pipe_exit_on_failure(fds)               \
    Sys_call_1_exit_on_failure(pipe, fds);       \

#define Kill_exit_on_failure(pid, signum)               \
    Sys_call_2_exit_on_failure(kill, pid, signum)       \

#define Dup2_exit_on_failure(fd1, fd2)          \
    Sys_call_2_exit_on_failure(dup2, fd1, fd2); \

#define Write_exit_on_failure(fd, buf, size)            \
    Sys_call_3_exit_on_failure(write, fd, buf, size);   \

#define Read_exit_on_failure(fd, buf, size)             \
    Sys_call_3_exit_on_failure(read, fd, buf, size);    \

#define Signal_exit_on_failure(signum, sig_handler)     \
    if (signal(signum, sig_handler) == SIG_ERR) {       \
        perror("Error using signal(). Exiting.");       \
        exit(EXIT_FAILURE);                             \
    }                                                   \
