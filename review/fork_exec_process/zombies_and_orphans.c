#include <unistd.h>
#include <stdlib.h>

#ifdef CHILD_DIES
  #define TOGGLE !
#else
  #define TOGGLE
#endif

int main() {
  if (TOGGLE fork()) {
    exit(0);
  }

  while(1)
    ;
}
