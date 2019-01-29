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
	if (input_str == NULL) return NULL;
 	// dimension 1 = # sentences
 	// dimension 2 = # chars in sentence
 	int punct_count = 0;
 	int double_punct = 0;
 	size_t i;

 	for (i = 0; i < strlen(input_str); i++) {
 		//		if (is_letter(input_str[i])) {
 		//			double_punct = 0;
 		//		}
 		if (ispunct(input_str[i]) && !double_punct)  {
 			punct_count++;
 			//			double_punct = 1;
 		}
 	}

 	int num_sentences = punct_count + 1;
 	char** result = (char**) malloc((sizeof(char*) * num_sentences) + 1);
 	result[num_sentences] = NULL;


 	double_punct = 0;
 	int char_count = 0;
 	size_t j = 0;
 	int anything = 0;

 	for (i = 0; i < strlen(input_str); i++) {
 		//		if (is_letter(input_str[i]) || input_str[i] == 32) {
 		char_count++;
 		//			double_punct = 0;
 		//		}
 		if ((ispunct(input_str[i]) && !double_punct)) {
 			anything = 1;
 			result[j] = malloc(char_count + 1);
 			//printf("result[%d] has %d\n", j, char_count + 1 );
 			j++;
 			char_count = 0;
 			//			double_punct = 1;
 		}
 	}


 	// final sentence
 	// used to be j+1
 	//	result[j] = malloc(char_count + 1);
 	double_punct = 0;
 	char_count = 0;
 	j = 0;


 	for (i = 0; i < strlen(input_str); i++) {
 		//		if (is_letter(input_str[i]) || input_str[i] == 32) {
 		// printf("top of loop j = %d\n", j);
 		// printf("ns %d\n", num_sentences);
 		if (j >= (size_t) (num_sentences - 1)) break;
 		if (!anything) break;

 		result[j][char_count] = input_str[i];
 		//printf("result[%d][%d] = input_str[%d] : %c\n", j, char_count,i, input_str[i]);
 		char_count++;
 		//			double_punct = 0;
 		//		}

 		if (ispunct(input_str[i]) /*&& !double_punct*/) {

 			//if (j == 4) printf("%d\n", char_count);

 			result[j][char_count] = 0;
 			//if (j == 4) printf("\n\nASSIGNED LAST NULL\n\n");
 			//printf("result[%d] = %s\n", j, result[j]);
 			//printf("btm of loop j = %d\n", j);
 			j++;
 			char_count = 0;
 			//double_punct = 1;
 		}
 		//if (i==strlen(input_str) )printf("i = %d, time to leave\n\n", i);
 	}

 	// set null for end of final sentence
 	//result[j][char_count] = 0;
 	//printf("result[%d] = %s\n", j, result[j]);


 	for (i = 0; i < (size_t)num_sentences; i++) {
 		result[i] = camel_caser_hf(result[i]);
 	}

 	return result;
}
char *camel_caser_hf( char *input_str) {
	if (input_str == NULL || strlen(input_str) == 0) {
 		return NULL;
 	}
 	input_str = capitalizer(input_str);
 	input_str = unspacer(input_str);
 	return input_str;
}

// helper function to capitalize all words but first
char* capitalizer( char* input_str) {
	size_t j;
 	for (j = 0; j < strlen(input_str); j++) {
 		input_str[j] = tolower(input_str[j]);
 	}

 	int cap = 0;
 	int have_we_hit_a_letter_yet = 0;

 	size_t i;
 	for (i = 0; i < strlen(input_str); i++) {


 		if (cap && is_letter(input_str[i]) && have_we_hit_a_letter_yet) {
 			input_str[i] -= 32;
 			cap = 0;

 		} else if (input_str[i] == 32) {
 			cap = 1;
 		}
 		if ((!have_we_hit_a_letter_yet) && is_letter(input_str[i])){
 			have_we_hit_a_letter_yet = 1;
 			cap = 0;
 		}
 	}

 	return input_str;
}

char *unspacer(char* str) {
	size_t i;
 	for (i = 0; i < strlen(str); i++) {
 		if (!is_letter(str[i]) && !isdigit(str[i])) {
 			memmove(&str[i], &str[i + 1], strlen(str) - i);
 			i--;
 		}
 	}

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
//  free(result);
}
