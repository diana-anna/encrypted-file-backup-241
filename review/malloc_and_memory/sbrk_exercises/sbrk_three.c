#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
  char* memory = sbrk(3*sizeof(unsigned int) + sizeof(int) + sizeof(int *) + 12);

  unsigned int * sizePtrOne = (unsigned int*) (memory);
  *sizePtrOne = sizeof(int);
  int* intPtr = (int *)(sizePtrOne + 1);
  *intPtr = 241;

  unsigned int * sizePtrTwo = (unsigned int*) (memory + sizeof(unsigned int) + sizeof(int));
  *sizePtrTwo = sizeof(int *);
  int** addressPtr = (int **)(sizePtrTwo + 1);
  *addressPtr = intPtr;

  unsigned int * sizePtrThree = (unsigned int*) (memory + 2*sizeof(unsigned int) + sizeof(int) + sizeof(int*));
  *sizePtrThree = 12;
  char* string = (char *)(sizePtrThree + 1);
  strcpy(string, "Hello world");

  printf("%p", sbrk(0));

  return 0;
}
