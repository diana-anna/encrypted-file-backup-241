#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
  unsigned int * sizePtrOne = sbrk(/* SOME VALUE HERE */);
  *sizePtrOne = /* SOME VALUE HERE */;
  int* intPtr = (int *)(sizePtrOne + /* SOME VALUE HERE */);
  *intPtr = 241;

  unsigned int * sizePtrTwo = sbrk(/* SOME VALUE HERE */);
  *sizePtrTwo = /* SOME VALUE HERE */;
  int** addressPtr = (int **)(sizePtrTwo + /* SOME VALUE HERE */);
  *addressPtr = intPtr;

  unsigned int * sizePtrThree = sbrk(/* SOME VALUE HERE */);
  *sizePtrThree = /* SOME VALUE HERE */;
  char* string = (char *)(sizePtrThree + /* SOME VALUE HERE */);
  strcpy(string, "Hello world");

  printf("%p", sbrk(/* SOME VALUE HERE */));

  return 0;
}
