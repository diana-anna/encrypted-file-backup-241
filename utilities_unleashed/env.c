/**
 * Utilities Unleashed Lab
 * CS 241 - Spring 2019
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "format.h"
#include <ctype.h>

int is_valid(char* arg) {
    char* temp = arg;
    while (*temp) {
	if (!isalpha(*temp) && !isdigit(*temp) && (*temp) != '_') {
	    return 0;
	}
	temp++;
    }
    return 1;
}

char** strsplit(char *str, char *delimiter) {
    char *copy = strdup(str);
	char *curr = strtok(copy, delimiter);
	int count = 0;
	while (curr) {
		//	printf("%s\n", curr);
		count++;
		curr = strtok(NULL, delimiter);
	}
	free(copy);
	char** result = malloc(sizeof(char*) * count + 1);
	
	// now do it again
	char* copy2 = strdup(str);
	char* curr2 = strtok(copy2, delimiter);
	int idx = 0;
	while (curr2) {
		//printf("%s\n", curr2);
		result[idx] = malloc(sizeof(char) * strlen(curr2));
		strcpy(result[idx], curr2);
		curr2 = strtok(NULL, delimiter);
		idx++;
	}
	free(copy2);
	return result;
}

int num_eVars(char* str, char* delimiter) {
	char *copy = strdup(str);
	char *curr = strtok(copy, delimiter);
	int count = 0;
	while (curr) {
		//	printf("%s\n", curr);
		count++;
		curr = strtok(NULL, delimiter);
	}
	free(copy);
	return count;
}
 
int main(int argc, char *argv[]) {
    if (argc == 1) {
	char** temp = environ;
	while (*temp) {
	    printf("%s\n", *temp);
	    temp++;
	}
	exit(0);
    }

    if (argc == 2) {
	print_env_usage();
	exit(1);
    }

    pid_t child_id = fork();
    if (child_id == -1) {
	print_fork_failed();
	exit(1);
    } else if (child_id == 0) {
	char** args = strsplit(argv[1], ",");
	int args_len = num_eVars(argv[1], ",");
	size_t i;
	size_t j;
	for (i = 0; i < (size_t)args_len; i++) {
	    for (j = 0; j < strlen(args[i]); j++) {
		if (!is_valid(&args[i][j])) exit(1);
		// counts index of first =
 	        if (args[i][j] == '=') {
		    break;
	        }
	    }
	    char* lhs = strndup(args[i], j);
	    size_t k;
	    char * result;
	    for (k = j + 1; k < strlen(args[i]); k++) {
		if (args[i][i] == '%') {
		    while (is_valid(&args[i][k + 1])) k++;
		    char* env_name = strndup(&args[i][j+2], &args[i][j+2] - &args[i][k] + 1);
		    if (getenv(env_name)) {
			result = realloc(result, strlen(result) + strlen(getenv(env_name) + 1));
		    }
		} else {
		    result = realloc(result, strlen(result) + 1);
		}
	    }
	    setenv(lhs, result, 1);
	    free(lhs);
	    free(result);
        }

	if (execvp(argv[2], &argv[4]) < 0) {
	    print_exec_failed();
	    exit(1);
	}

	// free before child dies
	
        for (i = 0; i < (size_t)args_len; i++) {
            free(args[i]);
        }
        free(args);
    } else {
	int status;
	waitpid(child_id, &status, 0);
    }
	
    return 0;
}
