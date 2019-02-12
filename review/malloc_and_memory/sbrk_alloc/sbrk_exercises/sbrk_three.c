#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
  char* memory = sbrk(/* SOME VALUE HERE*/);

  unsigned int * sizePtrOne = (unsigned int*) (memory + /* SOME VALUE HERE */);
  *sizePtrOne = sizeof(int);
  int* intPtr = (int *)(sizePtrOne + 1);
  *intPtr = 241;

  unsigned int * sizePtrTwo = (unsigned int*) (memory + /* SOME VALUE HERE */)
  *sizePtrTwo = sizeof(int *);
  int** addressPtr = (int **)(sizePtrTwo + 1);
  *addressPtr = intPtr;

  unsigned int * sizePtrThree = (unsigned int*) (memory + /* SOME VALUE HERE */);
  *sizePtrThree = 12;
  char* string = (char *)(sizePtrThree + 1);
  strcpy(string, "Hello world");

  printf("%p", sbrk(/* SOME VALUE HERE */));

  return 0;
}
