/**
 * Mini Valgrind Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Your tests here using malloc and free
    char* test1 = malloc(10);
    free(test1);

    char* test2 = calloc(6, 10);
    
    test2 = realloc(test2, 70);
    free(test2);
    
    char* test3 = malloc(70);
    test3 = realloc(test3, 70);
    free(test3);

    char* test4 = malloc(90);
    test4 = realloc(test4, 30);
    free(test4);

    char* test5 = calloc(2, 20);
    test5 = realloc(test5, 0);
    free(test5);

    return 0;
}
