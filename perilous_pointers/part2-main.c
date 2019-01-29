/**
 * Perilous Pointers Lab
 * CS 241 - Spring 2019
 */
 
#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
    // your code here

	first_step(81);
	
	int val[] = {132};
	second_step(val);
	int **val2 = (int**) malloc(sizeof(int*) * 4);
	val2[0] = malloc(sizeof(int) * 2);
	val2[0][0] = 8942;
	double_step(val2);
	free(val2);
	
	char *val3 = malloc(6 * sizeof(int));
	*(int*)(val3+5) = 15;
	strange_step(val3);
	free(val3);
	
	char *val4 = malloc(4 * sizeof(char));
	val4[3] = 0;
	empty_step((void*)val4);
	free(val4);
	
	char *val5_1 = malloc(4 * sizeof(char));
	val5_1[3] = 'u';
	void *val5_2 = val5_1;
	two_step(val5_2, val5_1);
	free(val5_1);
	
	char *first = malloc(5 * sizeof(char));
	three_step(first, first + 2, first + 4);
	free(first);
	
	char *a = malloc(2 * sizeof(char));
	a[1] = 0;
	char *b = malloc(3 * sizeof(char));
	b[2] = 8;
	char *c = malloc(4 * sizeof(char));
	c[3] = 16;
	step_step_step(a,b,c);
	free(a);
	free(b);
	free(c);
	
	char* a_ = malloc(1 * sizeof(char));
	a_[0] = 'a';
	int b_ = 97;
	it_may_be_odd(a_, b_);
	free(a_);
	
	char arr[] = "hello,CS241";
	tok_step(arr);

	int *i = malloc(sizeof(int));
    	*i = 0x201;
    	the_end((void *) i, (void *) i);
    	free(i);

    return 0;
}
