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

    vector* test_v = vector_create(char_copy_constructor, char_destructor, char_default_constructor);
    vector_push_back(test_v, "this");
    vector_push_back(test_v, "is");
    vector_push_back(test_v, "a");
    vector_push_back(test_v, "sentence");
    sstring *test_ss4 = cstr_to_sstring("this is a sentence");
    vector* test_v_out = sstring_split(test_ss4, ' ');
    assert(*(char*)vector_get(test_v,0) == *(char*)vector_get(test_v_out,0));
    assert(*(char*)vector_get(test_v,1) == *(char*)vector_get(test_v_out,1));
    assert(*(char*)vector_get(test_v,2) == *(char*)vector_get(test_v_out,2));
    assert(*(char*)vector_get(test_v,3) == *(char*)vector_get(test_v_out,3));

    sstring *slice_me = cstr_to_sstring("1234567890");
    char* sliced = sstring_slice(slice_me, 2, 5);
//    printf("in test: %s\n", sliced);
    assert(!strcmp(sliced,"345"));

    sstring_substitute(test_ss4, 4, "sentence", "butt");

//    size_t j;
    //char* aa = sstring_to_cstr(test_ss4);
    return 0;
}
