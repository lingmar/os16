/**
 * Game of luck: Implementation of the Gamemaster
 *
 * Course: Operating Systems and Multicore Programming - OSM lab
 * assignment 1: game of luck.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */

#include <stdio.h> /* I/O functions: printf() ... */
#include <stdlib.h> /* rand(), srand() */
#include <unistd.h> /* read(), write() calls */
#include <assert.h> /* assert() */
#include <time.h>   /* time() */
#include <signal.h> /* kill(), raise() and SIG???? */

#include <sys/types.h> /* pid */
#include <sys/wait.h> /* waitpid() */

#include "common.h"
#include "system_calls.h"

#define Write_fd(i) (2 * (i) + 1)
#define Read_fd(i) (2 * (i))

int main(int argc, char *argv[]) {
    int i, seed;

    /* TODO: Use the following variables in the exec system call. Using the */
    /* function sprintf and the arg1 variable you can pass the id parameter */
    /* to the children */
    char arg0[] = "./shooter";
    char arg1[10];
    char *args[] = {arg0, arg1, NULL};
    
    /* TODO: initialize the communication with the players */
    int pfds_seed[2*NUM_PLAYERS];
    int pfds_score[2*NUM_PLAYERS];
    
    for (i = 0; i < NUM_PLAYERS; i++) {
        pipe_exit_on_failure(&(pfds_seed[2 * i]));
        pipe_exit_on_failure(&(pfds_score[2 * i]));
    }
    
    pid_t pids[NUM_PLAYERS];
    for (i = 0; i < NUM_PLAYERS; i++) {
        /* TODO: spawn the processes that simulate the players */
        
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("Error using fork(). Exiting.");
            exit(EXIT_FAILURE);
        }
        else if (pids[i] != 0) {
            close_exit_on_failure(pfds_seed[Read_fd(i)]);
            close_exit_on_failure(pfds_score[Write_fd(i)]);
        }
        else {
            dup2_exit_on_failure(pfds_seed[Read_fd(i)],
                                 STDIN_FILENO);
            dup2_exit_on_failure(pfds_score[Write_fd(i)],
                                 STDOUT_FILENO);
            
            close_exit_on_failure(pfds_seed[Write_fd(i)]);
            close_exit_on_failure(pfds_seed[Read_fd(i)]);
            close_exit_on_failure(pfds_score[Write_fd(i)]);
            close_exit_on_failure(pfds_score[Read_fd(i)]);
            
            sprintf(arg1, "%d", i);
            execv(args[0], args);

            perror("No return expected. Must be error with execv().");
            exit(EXIT_FAILURE);
        } 
    }

    seed = time(NULL);
    for (i = 0; i < NUM_PLAYERS; i++) {
        seed++;
        /* TODO: send the seed to the players */

        write_exit_on_failure(pfds_seed[Write_fd(i)], &seed, sizeof(int));
    }

    /* TODO: get the dice results from the players, find the winner */
    /* FIXME: is the results always positive? */
    int highest = -1; 
    int score;
    for (i = 0; i < NUM_PLAYERS; i++) {

        read_exit_on_failure(pfds_score[Read_fd(i)], &score, sizeof(int));

        if (score > highest) {
            winner = i;
            highest = score;
        }
    }
    printf("master: player %d WINS\n", winner);

    /* TODO: signal the winner */
    kill_exit_on_failure(pids[winner], SIGUSR1);
    
    /* TODO: signal all players the end of game */
    for (i = 0; i < NUM_PLAYERS; i++) {
        kill_exit_on_failure(pids[i], SIGUSR2);
    }

    printf("master: the game ends\n");

    /* TODO: cleanup resources and exit with success */
    int result;
    for (i = 0; i < NUM_PLAYERS; i++) {
        int pid = wait(&result);
        if (pid < 0) {
            perror("Error using wait(). Exiting");
            exit(EXIT_SUCCESS);
        }
    }

    exit(EXIT_SUCCESS);
    
    return 0;
}
