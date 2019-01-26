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
	// dimension 1 = # sentences
	// dimension 2 = # chars in sentence
	return NULL;
}

char *camel_caser_hf( char *input_str) {
	if (input_str == NULL) {
		return NULL;
	}

	input_str = capitalizer(input_str);
	input_str = unspacer(input_str);
	return input_str;

	// int start_index = 0;
	// int punct_index = 0;
	// while (*input_str) {
	// 	char* sentence;
	// 	while (*input_str && (is_letter(*input_str) || *input_str == 32)) { // while it is a letter or space
	// 	punct_index++;
	// 	input_str++;
	// 	}
	// 	// from start_index to punct_index is a sentence
	// 	memmove(&sentence, &input_str - punct_index, punct_index);

	// }

}

// helper function to capitalize all words but first
char* capitalizer( char* input_str) {
	char capitalized[strlen(input_str) + 1]; // editable copy
	strcpy(capitalized, input_str);

	size_t j;
	for (j = 0; j < strlen(capitalized); j++) {
		capitalized[j] = tolower(capitalized[j]);
	}

	int cap = 0;
	int have_we_hit_a_letter_yet = 0;

	size_t i;
	for (i = 0; i < strlen(input_str); i++) {


		if (cap && is_letter(capitalized[i]) && have_we_hit_a_letter_yet) {
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
	input_str = strdup(capitalized);

	return input_str;
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
	// char* result = malloc(strlen(spaced) + 1);
	// strcpy(result, spaced);

	str = strdup(spaced);

	return str;
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
