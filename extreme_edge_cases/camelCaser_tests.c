/**
 * Extreme Edge Cases Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camelCaser.h"
#include "camelCaser_tests.h"

/*
 * Testing function for various implementations of camelCaser.
 *
 * @param  camelCaser   A pointer to the target camelCaser function.
 * @param  destroy      A pointer to the function that destroys camelCaser
 * output.
 * @return              Correctness of the program (0 for wrong, 1 for correct).
 */
int test_camelCaser(char **(*camelCaser)(const char *),
                    void (*destroy)(char **)) {
    // TODO: Return 1 if the passed in function works properly; 0 if it doesn't.
	size_t i;

 	// null
 	char** result0 = (*camelCaser)(NULL);
 	if (result0) return 0;

 	// "t.his clASS I*S so Fu*CKing ha/rd. jesus chrsit";
 	char** solution1 = malloc((6 * sizeof(char*)));
 	solution1[0] = "t";
 	solution1[1] = "hisClassI";
 	solution1[2] = "sSoFu";
 	solution1[3] = "ckingHa";
 	solution1[4] = "rd";
 	solution1[5] = NULL;

 	char** result1 = (*camelCaser)("t.his clASS I*S so Fu*CKing ha/rd. jesus chrsit");

 	if (sizeof(result1) !=  sizeof(solution1)) {

 		(*destroy)(solution1);
 		// for (i = 0; i < 6; i++) {
 		// 	free(solution1[i]);
 		// }
 		// free(solution1);

 		return 0;
 	}

 	for (i = 0; i < sizeof(result1) / sizeof(char*); i++) {
 		if (strcmp(result1[i], solution1[i]) != 0) {

 			(*destroy)(solution1);
 			return 0;
 		}
 	}
 	//printf("hi\n");
 	(*destroy)(solution1);

 	// "1a2b3c4d5e6f7g"
 	char** result2 = (*camelCaser)("1a2b3c4d5e6f7g");
 	if (*result2) return 0;

 	// ".. . . he$l*&lo wo.rl/d"
 	char** solution3 = malloc(10 * sizeof(char*));
 	solution3[0] = "";
 	solution3[1] = "";
 	solution3[2] = "";
 	solution3[3] = "";
 	solution3[4] = "he";
 	solution3[5] = "l";
 	solution3[6] = "";
 	solution3[7] = "loWo";
 	solution3[8] = "rl";
 	solution3[9] = NULL;

 	char** result3 = (*camelCaser)(".. . . he$l*&lo wo.rl/d");

 	if (sizeof(result3) != sizeof(solution3)) {
 		(*destroy)(solution3);
 		return 0;
 	}

 	for (i = 0; i < sizeof(result3) / sizeof(char*); i++) {
 		if (strcmp(result3[i], solution3[i]) != 0) {
 			(*destroy)(solution3);
 			return 0;
 		}
 	}

 	(*destroy)(solution3);

 	// "\\\\\\ahi."
 	char** solution4 = malloc(5 * sizeof(char*));

 	solution4[0] = "";
 	solution4[1] = "";
 	solution4[2] = "";
 	solution4[3] = "ahi";
 	solution4[4] = NULL;

 	char** result4 = (*camelCaser)("\\\\\\ahi.");

 	if (sizeof(result4) != sizeof(solution4)) {
 		(*destroy)(solution4);
 		return 0;
 	}

 	for (i = 0; i < 5; i++) {
 		if (result4[i] != solution4[i]) {
 			(*destroy)(solution4);
 			return 0;
 		}
 	}

 	(*destroy)(solution4);


 	// "\n\n\nHI what's up? hello. ho.la"
 	char **solution5 = malloc(5 * sizeof(char*));

 	solution5[0] = "hiWhat";
 	solution5[1] = "sUp";
 	solution5[2] = "hello";
 	solution5[3] = "ho";
 	solution5[4] = NULL;

 	char** result5 = (*camelCaser)("\n\n\nHI what's up? hello. ho.la");

 	if (sizeof(result5) != sizeof(solution5)) {
 		(*destroy)(solution5);
 		return 0;
 	}

 	for (i = 0; i < 5; i++) {
 		if (result5[i] != solution5[i]) {
 			(*destroy)(solution5);
 			return 0;
 		}
 	}

 	(*destroy)(solution5);
 	return 1;
 	
}
