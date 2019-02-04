/**
 * Vector Lab
 * CS 241 - Spring 2019
 */
 
#include "sstring.h"
#include "vector.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <string.h>

struct sstring {
    // Anything you want
    vector *v;
};

sstring *cstr_to_sstring(const char *input) {
    // your code goes here
    sstring *s_str = (sstring*) malloc(sizeof(sstring));
    s_str->v = vector_create(&string_copy_constructor, &string_destructor, &string_default_constructor);
    size_t i;
    for (i = 0; i < strlen(input); i++) {
      char curr = input[i];
      //vector_set(s_str->v, i, &curr);
      vector_push_back(s_str->v, &curr);
    }
    return s_str;
}

char *sstring_to_cstr(sstring *input) {
    // your code goes here
    size_t sz = vector_size(input->v);
    char* cstr = (char*) malloc(sz * sizeof(char));
    size_t i;
    for (i = 0; i < sz; i++) {
      cstr[i] = *(char*) vector_get(input->v, i);
    }
    return cstr;
}

int sstring_append(sstring *this, sstring *addition) {
    // your code goes here
    size_t i;
    size_t sz = vector_size(addition->v);
    for (i = 0; i < sz; i++) {
      vector_push_back(this->v, &addition[i]);
    }
    return vector_size(this->v);
}

vector *sstring_split(sstring *this, char delimiter) {
    // your code goes here
    return NULL;
}

int sstring_substitute(sstring *this, size_t offset, char *target,
                       char *substitution) {
    // your code goes here
    return -1;
}

char *sstring_slice(sstring *this, int start, int end) {
    // your code goes here
    return NULL;
}

void sstring_destroy(sstring *this) {
    // your code goes here
}
