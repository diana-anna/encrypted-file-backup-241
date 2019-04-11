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
//	printf("%s\n", word);
        free(word);
      }
    }
    // final word:
    // plus 1 because count increments at beginning of loop and we exited
    char* word = malloc((sizeof(char) * count) + 1);
    memmove(word, &this_cstr[i - (count)], count);
    word[count] = 0;
    vector_push_back(split, word);
//    printf("%s\n", word);
    free(word); 

    return split;
}

int sstring_substitute(sstring *this, size_t offset, char *target,
                       char *substitution) {
    // your code goes here
    char* ss_char = sstring_to_cstr(this);
    if (offset == strlen(ss_char) - 1) return -1;

    char* result = malloc((sizeof(char) * strlen(ss_char)) + strlen(substitution) - strlen(target) + 1);
    size_t k;
    size_t i = 0;
    int flag = 0;
    for (k = 0; k < strlen(ss_char); k++) {
    if ((strstr(&ss_char[k], target) == &ss_char[k]) && k > offset && !flag) {    
        flag = 1;
        strcpy (&result[i], substitution);
        i+=strlen(substitution);
	k += strlen(target);
      } else {
        result[i] = ss_char[k];
	i++;
      }
    }
   // result[i + strlen(substitution)] = 0;
    strcpy(ss_char, result);
    //ss_char[i + strlen(substitution)] = 0;
    vector_destroy(this->v);
    
//    printf("%s\n", ss_char);  
    for(i = 0; i < strlen(result); i++) {
	vector_push_back(this->v, &result[i]);
    }
    free(result);
    if (flag) return 0;

    free(result);
    return -1;
}

char *sstring_slice(sstring *this, int start, int end) {
    // your code goes here
    assert(start <= end);

    char* slice = malloc((sizeof(char) * (end - start)) + 1);
    char* this_cstr = sstring_to_cstr(this);
    assert((size_t)end < strlen(this_cstr));
    memmove(slice, &this_cstr[start], end-start);
//    printf("in impl: %s\n", slice);
    return slice;
}

void sstring_destroy(sstring *this) {
    // your code goes here
    vector_clear(this->v);
    vector_destroy(this->v);
    free(this);
}
