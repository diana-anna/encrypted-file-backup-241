/*
 * Vector Lab
 * CS 241 - Spring 2019
 */
 
#include "sstring.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // TODO create some tests
    char* test = "this is a sentence";
    sstring* test_ss = cstr_to_sstring(test);
    char* test2 = sstring_to_cstr(test_ss);
    assert(!strcmp(test, test2));
    
    sstring* test_ss2 = cstr_to_sstring("hi");
    sstring* test_ss3 = cstr_to_sstring("hola");
    assert(sstring_append(test_ss2, test_ss3) == 6);
    assert(vector_size(get_vector(test_ss2)) == 6);
    assert(*(char*)vector_get(get_vector(test_ss2), 0) == 'h');
    assert(*(char*)vector_get(get_vector(test_ss2), 1) == 'i');
    assert(*(char*)vector_get(get_vector(test_ss2), 2) == 'h');
    assert(*(char*)vector_get(get_vector(test_ss2), 3) == 'o');
    assert(*(char*)vector_get(get_vector(test_ss2), 4) == 'l');
    assert(*(char*)vector_get(get_vector(test_ss2), 5) == 'a');

    return 0;
}
