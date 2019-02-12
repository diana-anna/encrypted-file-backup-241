#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
  int* intPtr = sbrk(/* SOME VALUE HERE */);
  *intPtr = 241;

  int** addressPtr = sbrk(/* SOME VALUE HERE */);
  *addressPtr = intPtr;

  char* string = sbrk(/* SOME VALUE HERE */);
  strcpy(string, "Hello world");

  printf("%p", sbrk(/* SOME VALUE HERE */));

  return 0;
}
