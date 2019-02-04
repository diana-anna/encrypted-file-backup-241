/**
 * Vector Lab
 * CS 241 - Spring 2019
 */

#include "vector.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Write your test cases here

    vector *v = vector_create(&int_copy_constructor, &int_destructor, &int_default_constructor);
    assert(vector_size(v) == (size_t) 0);
    assert(vector_capacity(v) == (size_t) 8);

    int int_test = 7;
    vector_push_back(v, &int_test);
    assert(vector_size(v) == 1);

//    printf("(int)*vector_get(v, 0) = %d\n", *(int*)(vector_get(v,0)));
  //  printf("(int)*vector_begin(v) = %d\n",(int)(*vector_begin(v)));
   // printf("*(int*)vector_at(v,0) = %d\n", *(int*)vector_at(v,0));
   // printf("(int)*vector_front(v) = %d\n",(int)(*vector_front(v)));    

    assert(*(int*)vector_get(v,0) == 7);

    vector_resize(v, 10);
    assert(vector_size(v) == 1);
    assert(vector_capacity(v) == 16);

    int int_test2 = 9;
    vector_set(v, 0, &int_test2);
    assert(vector_size(v) == 1);
    assert(vector_capacity(v) == 16);
    assert(*(int*)vector_get(v,0) == 9); 

    int int_test3 = 4;
    int int_test4 = 8;
    int int_test5 = 14;
    int int_test6 = 54;
    int int_test7 = 98;
    int int_test8 = 1;
    int int_test9 = 32;
    int int_test10 = 65;
    int int_test11 = 34;
    int int_test12 = 64;
    int int_test13 = 3;
    int int_test14 = 24;
    int int_test15 = 5;
    int int_test16 = 80;

    vector_push_back(v, &int_test);
    vector_push_back(v, &int_test3);
    vector_push_back(v, &int_test4);
    vector_push_back(v, &int_test5);
    vector_push_back(v, &int_test6);
    vector_push_back(v, &int_test7);
    vector_push_back(v, &int_test8);
    vector_push_back(v, &int_test9);
    vector_push_back(v, &int_test10);
    vector_push_back(v, &int_test11);
    vector_push_back(v, &int_test12);
    vector_push_back(v, &int_test13);
    vector_push_back(v, &int_test14);
    vector_push_back(v, &int_test15);
    vector_push_back(v, &int_test16);

    assert(vector_size(v) == 16);

    vector_destroy(v);

    return 0;
}

