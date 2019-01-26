/**
 * Extreme Edge Cases Lab
 * CS 241 - Spring 2019
 */
 
#include "camelCaser.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char **camel_caser(const char *input_str) {
	return NULL;
}

// helper function to capitalize all words but first
char *capitalizer(const char *input_str) {
    char *capitalized = malloc(strlen(input_str) + 1);
    strcpy(capitalized, input_str);

    size_t j;
    for (j = 0; j < strlen(capitalized); j++) {
	capitalized[j] = tolower(capitalized[j]);
    }

    int cap = 0;
    int have_we_hit_a_letter_yet = 0; // account for extra spaces
    size_t i;
    for (i = 0; i < strlen(input_str); i++) {
          if (cap && is_letter(capitalized[i])) {
            capitalized[i] -= 32;
            cap = 0;
          } else if (capitalized[i] == 32) {
            cap = 1;
          }
	  if ((!have_we_hit_a_letter_yet) && is_letter(capitalized[i])){
	    have_we_hit_a_letter_yet = 1;
	    cap = 0;
          }
    }
    char* result = malloc(strlen(input_str) + 1);
    strcpy(result, capitalized);
    return result;
}

char *unspacer(char* str) {
	char spaced[strlen(str) + 1];
	strcpy(spaced, str);
	size_t i;
	for (i = 0; i < strlen(spaced); i++) {
		//	printf("i = %d : current char = %c\n", i, spaced[i]);
		if (!is_letter(spaced[i])) {
			//	printf("\nspace\n\n");
			memmove(&spaced[i], &spaced[i + 1], strlen(spaced) - i);
			//	printf("\nspaced = %s\n\n", spaced);
			i--;
		}
	}
	char* result = malloc(strlen(spaced) + 1);
	strcpy(result, spaced);
	
	return result;
}

// helper function to check if a char is a letter
int is_letter(char str) {
  if ((str >= 65 && str <= 90)
      || (str >= 97 && str <= 122)) {
    return 1;
  }
  return 0;
}

void destroy(char **result) {
    return;
}
