/**
 * Teaching Threads Lab
 * CS 241 - Spring 2019
 */
 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reduce.h"
#include "reducers.h"

/* You might need a struct for each task ... */
typedef struct task {
    int* list;
    int list_len;
    int* list_head;
    reducer reduce_func;
    int base_case;
} task;

static int current = 0;

/* You should create a start routine for your threads. */
void* start_routine(void* arg) {
    task* t = (task*) arg;
    t->list_head[current] = reduce(t->list, t->list_len, t->reduce_func, t->base_case);
    current++;
    return NULL;
}

/* FUNCTION TO OPTIMIZE
int reduce(int *list, size_t length, reducer reduce_func, int base_case) {
    int result = base_case;

    for (size_t i = 0; i < length; ++i) {
        result = reduce_func(result, list[i]);
    }

    return result;
}*/

int par_reduce(int *list, size_t list_len, reducer reduce_func, int base_case,
               size_t num_threads) {
    /* Your implementation goes here */
    size_t i;
    size_t elems_per_thread = list_len / num_threads;
    task tasks[num_threads];
    pthread_t pthreads[num_threads];
    int return_list[num_threads];

    for (i = 0; i < num_threads; i++) {
        tasks[i].reduce_func = reduce_func;
        tasks[i].base_case = base_case;
        tasks[i].list = list + (i * elems_per_thread);
        tasks[i].list_len = elems_per_thread;
        tasks[i].list_head = return_list;
    }

    i--;
    tasks[i].list_len += list_len % num_threads;

    for (i = 0; i < num_threads; i++) {
        pthread_create(&pthreads[i], NULL, &start_routine, &tasks[i]);
    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(pthreads[i], NULL);
    }
    
    return reduce(return_list, num_threads, reduce_func, base_case);
}
