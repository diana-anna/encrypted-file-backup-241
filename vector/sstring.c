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

vector* get_vector(sstring* s_str) {
  return s_str->v;
}

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
    char* cstr = (char*) malloc((sz * sizeof(char)) + 1);
    size_t i;
    for (i = 0; i < sz; i++) {
      cstr[i] = *(char*) vector_get(input->v, i);
    }
    cstr[i] = 0;
    return cstr;
}

int sstring_append(sstring *this, sstring *addition) {
    // your code goes here
    size_t i;
    size_t sz = vector_size(addition->v);
    for (i = 0; i < sz; i++) {
      vector_push_back(this->v, vector_get(addition->v, i));
    }
    //printf("%s\n", sstring_to_cstr(this));
    return vector_size(this->v);
}

vector *sstring_split(sstring *this, char delimiter) {
    // your code goes here
    char* this_cstr = sstring_to_cstr(this);
    size_t begin = 0;
    vector* split = vector_create(char_copy_constructor, char_destructor, char_default_constructor);
    size_t i;
    int count = 0;
    for (i = 0; i < strlen(this_cstr); i++) {
      count++;
      if (this_cstr[i] == delimiter) {
        char* word = malloc((sizeof(char) * count) );
        memmove(word, &this_cstr[i - (count - 1)], count);
        word[count - 1] = 0;
        vector_push_back(split, word);
        begin = i;
        count = 0;
        free(word);
      }
    }
    // final word:
    // plus 1 because count increments at beginning of loop and we exited
    char* word = malloc((sizeof(char) * count) + 1);
    memmove(word, &this_cstr[i - (count - 1)], count);
    word[count] = 0;
    vector_push_back(split, word);
    free(word); 

    return split;
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
