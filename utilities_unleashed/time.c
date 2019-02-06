/**
 * Utilities Unleashed Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
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

    pid_t child_id = fork();
    printf("fork executed.\n");
    if (child_id == -1) {
	print_fork_failed();
	exit(1);
    }
    struct timespec begin, end;
    if (child_id) {
	int status;
	clock_gettime(CLOCK_MONOTONIC, &begin);
	waitpid(child_id, &status, 0);
	clock_gettime(CLOCK_MONOTONIC, &end);
        // do math stuff
     } else {
	if (execvp(argv[1], &argv[1]) < 0) {
	    print_exec_failed();
            exit(1);
	}
     }

    return 0;
}
