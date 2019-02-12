#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
  int* intPtr = sbrk(sizeof(int));
  *intPtr = 241;

  int** addressPtr = sbrk(sizeof(int *));
  *addressPtr = intPtr;

  char* string = sbrk(12);
  strcpy(string, "Hello world");

  printf("%p", sbrk(0));

  return 0;
}
