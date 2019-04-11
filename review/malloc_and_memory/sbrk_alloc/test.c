#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define M (1024*1024)
int main() {
  void *ptrs[100];
  for (int i = 0; i < 100; i++) {
    ptrs[i] = malloc(M);
    memset(ptrs[i], 1, M);
    if (i % 3 == 0) {
      free(ptrs[i/3]);
      ptrs[1/3] = NULL;
    }
    sleep(1);
  }

  for (int i = 0; i < 100; i++) {
    free(ptrs[i]);
  }
  return 0;
}
