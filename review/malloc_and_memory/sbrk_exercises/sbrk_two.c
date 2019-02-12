#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
  unsigned int * sizePtrOne = sbrk(sizeof(unsigned int) + sizeof(int));
  *sizePtrOne = sizeof(int);
  int* intPtr = (int *)(sizePtrOne + 1);
  *intPtr = 241;

  unsigned int * sizePtrTwo = sbrk(sizeof(unsigned int) + sizeof(int *));
  *sizePtrTwo = sizeof(int *);
  int** addressPtr = (int **)(sizePtrTwo + 1);
  *addressPtr = intPtr;

  unsigned int * sizePtrThree = sbrk(sizeof(unsigned int) + 12);
  *sizePtrThree = 12;
  char* string = (char *)(sizePtrThree + 1);
  strcpy(string, "Hello world");

  printf("%p", sbrk(0));

  return 0;
}
