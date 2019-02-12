#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
  char* memory = sbrk(/* SOME VALUE HERE */);

  unsigned int * sizePtrOne = (unsigned int*) (memory + /* SOME VALUE HERE */);
  *sizePtrOne = /* SOME VALUE HERE */;
  int* intPtr = (int *)(sizePtrOne + /* SOME VALUE HERE */);
  *intPtr = 241;

  unsigned int * sizePtrTwo = (unsigned int*) (memory + /* SOME VALUE HERE */);
  *sizePtrTwo = /* SOME VALUE HERE */;
  int** addressPtr = (int **)(sizePtrTwo + /* SOME VALUE HERE */);
  *addressPtr = intPtr;

  unsigned int * sizePtrThree = (unsigned int*) (memory + /* SOME VALUE HERE */);
  *sizePtrThree = /* SOME VALUE HERE */;
  char* string = (char *)(sizePtrThree + /* SOME VALUE HERE */);
  strcpy(string, "Hello world");

  printf("%p", sbrk(/* SOME VALUE HERE */));

  return 0;
}
