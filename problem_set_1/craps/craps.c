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

#include <string.h>

#include "common.h"

int main(int argc, char *argv[])
{
  int i, seed;

  /* TODO: Use the following variables in the exec system call. Using the
   * function sprintf and the arg1 variable you can pass the id parameter
   * to the children
   */
  
    char arg0[] = "./shooter";
    char arg1[10];
    char *args[] = {arg0, arg1, NULL};
  
  /* TODO: initialize the communication with the players */
  int seed_pfd[NUM_PLAYERS][2];
  int score_pfd[NUM_PLAYERS][2];
  for (i = 0; i < NUM_PLAYERS; i++) {

    int seed_pipe = pipe(seed_pfd[i]);
    int score_pipe = pipe(score_pfd[i]);

    if ((seed_pipe < 0) || (score_pipe < 0)) {
      fprintf(stderr,"Failed to allocate for pipes\n");
      exit(EXIT_FAILURE);
    }
  }

  pid_t pids[NUM_PLAYERS];
  for (i = 0; i < NUM_PLAYERS; i++) {
    /* TODO: spawn the processes that simulate the players */
        
    pids[i] = fork();
    if (pids[i] == -1)
      fprintf(stderr, "Something went wrong\n");
    else {
      if (pids[i] != 0) { 
	close(seed_pfd[i][0]);
	close(score_pfd[i][1]);
      }
      else if (pids[i] == 0) {
	close(seed_pfd[i][1]);
	close(score_pfd[i][0]);
	dup2(seed_pfd[i][0], STDIN_FILENO);
	dup2(score_pfd[i][1], STDOUT_FILENO);
	
	/*	close(seed_pfd[i][0]);
		close(score_pfd[i][1]);*/
	//shooter(i, seed_pfd[i][0], score_pfd[i][1]);

	//fprintf(stderr, "%d seed_rd, %d score_wr\n", seed_pfd[i][0], score_pfd[i][1]);
	sprintf(arg1, "%d", i);
	execv(args[0], args);
      }
    }
  }
  
  seed = time(NULL);
  for (i = 0; i < NUM_PLAYERS; i++) {
    seed++;
    /* TODO: send the seed to the players */
    int send = write(seed_pfd[i][1], &seed, sizeof(int));
     if (send < 0) {
      fprintf(stderr, "Parent failed to write to pipe\n");
      exit(EXIT_FAILURE);
    }	  
  }

  /* TODO: get the dice results from the players, find the winner */
  int max = 0;
  int cur;
  for (i = 0; i < NUM_PLAYERS; i++) {
    int receive = read(score_pfd[i][0], &cur, sizeof(int));
  
    if (receive < 0 ) {
      fprintf(stderr, "Parent failed to read from pipe\n");
      exit(EXIT_FAILURE);
    }
    if (max < cur) {
      winner = i;
      max = cur;
    }
  }
  fprintf(stderr, "master: player %d WINS\n", winner);

  /* TODO: signal the winner */
  kill(pids[winner], SIGUSR1);

  /* TODO: signal all players the end of game */
  for (i = 0; i < NUM_PLAYERS; i++) {
    kill(pids[i], SIGUSR2);
  }

  //printf("master: the game ends\n");

  /* TODO: cleanup resources and exit with success */
  int result;
  for (i = 0; i < NUM_PLAYERS; i++) {
    int pid = wait(&result);
    if (pid == -1)
      fprintf(stderr,"Something went wrong!\n");
  }

  return 0;
}
