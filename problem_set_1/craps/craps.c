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

#define WRITE_FD(i) (2 * i + 1)
#define READ_FD(i) (2 * i)

int main(int argc, char *argv[])
{
    int i, seed;

    /* TODO: Use the following variables in the exec system call. Using the
     * function sprintf and the arg1 variable you can pass the id parameter
     * to the children
     */
    /*
      char arg0[] = "./shooter";
      char arg1[10];
      char *args[] = {arg0, arg1, NULL};
    */
    /* TODO: initialize the communication with the players */
    int pfds_seed[2*NUM_PLAYERS];
    printf("%d\n", pfds_seed[0]);
    printf("%d\n", pfds_seed[1]);
    int pfds_score[2*NUM_PLAYERS];
    printf("%d\n", pfds_score[0]);
    printf("%d\n", pfds_score[1]);
    
    for (i = 0; i < NUM_PLAYERS; i++) {
        printf("File %s line %d\n", __FILE__, __LINE__);
        
        if (pipe(&(pfds_seed[2 * i])) < 0 ||
            pipe(&(pfds_score[2 * i])) < 0) {

            perror("Something went wrong with pipe()");
            exit(EXIT_FAILURE);
        }
    }
    
    pid_t pids[NUM_PLAYERS];
    for (i = 0; i < NUM_PLAYERS; i++) {
        /* TODO: spawn the processes that simulate the players */
        
        pids[i] = fork();
        if (pids[i] < 0)
            printf("Something went wrong\n");
        else if (pids[i] != 0) {
            close(pfds_seed[READ_FD(i)]);
            close(pfds_score[WRITE_FD(i)]);
        } else {
            close(pfds_seed[WRITE_FD(i)]);
            close(pfds_score[READ_FD(i)]);
            shooter(i, pfds_seed[READ_FD(i)], pfds_score[WRITE_FD(i)]);
        } 
    }

    seed = time(NULL);
    for (i = 0; i < NUM_PLAYERS; i++) {
        seed++;
        /* TODO: send the seed to the players */
        printf("Sending seed %d to player %d\n", seed, i);
        write(pfds_seed[WRITE_FD(i)], &seed, sizeof(int));
    }

    /* TODO: get the dice results from the players, find the winner */
    /* FIXME: is the results always positive? */
    int highest = -1; 
    int score;
    for (i = 0; i < NUM_PLAYERS; i++) {
        read(pfds_score[READ_FD(i)], &score, sizeof(int));
        if (score > highest) {
            winner = i;
            highest = score;
        }
    }
    printf("master: player %d WINS\n", winner);

    /* TODO: signal the winner */
    kill(pids[winner], SIGUSR1);
    
    /* TODO: signal all players the end of game */
    for (i = 0; i < NUM_PLAYERS; i++) {
        kill(pids[i], SIGUSR2);
    }

    printf("master: the game ends\n");

    /* TODO: cleanup resources and exit with success */
    int result;
    for (i = 0; i < NUM_PLAYERS; i++) {
        int pid = wait(&result);
        if (pid == -1)
            printf("Something went wrong!\n");
    }

    return 0;
}
