/**
 * Utilities Unleashed Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "format.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
	print_time_usage();
	exit(0);
    }
    
    char** child_argv = malloc(sizeof(char*) * argc + 1);
    size_t i;
    for (i = 1; i < (size_t) argc; i++) {
    	child_argv[i - 1] = argv[i];
    }
    child_argv[i - 1] = 0;
    
    pid_t child_id = fork();
    printf("fork executed.\n");
    
    if (child_id == -1) {
	print_fork_failed();
	exit(1);
    }

    struct timespec begin, end;
    if (child_id) { // parent
	printf("parent reached\n");
	int status;
	clock_gettime(CLOCK_MONOTONIC, &begin);
	waitpid(child_id, &status, 0);
	clock_gettime(CLOCK_MONOTONIC, &end);
	time_t seconds = end.tv_sec - begin.tv_sec;
	// for testing - DELETE LATER
	printf("seconds = %f\n", (double)seconds);
    } else { // child
	printf("child reached\n");
	if (execvp(child_argv[0], child_argv) < 0) {
	    print_exec_failed();
            exit(1);
	}
     }

    return 0;
}
