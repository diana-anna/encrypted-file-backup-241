
#include <unistd.h>
#include <string.h>

void *malloc(size_t size) {
  return sbrk(size);
}

void *calloc(size_t nmemb, size_t size) {
  void *ptr = sbrk(size * nmemb);
  if (ptr)
    memset(ptr, 0, size * nmemb);
  return ptr;
}

void *realloc(void *ptr, size_t size) {
  void *new_ptr = sbrk(size);
  if (new_ptr)
    memcpy(new_ptr, ptr, size);
  return new_ptr;
}

void free(void *ptr) {
  (void)ptr;
}
