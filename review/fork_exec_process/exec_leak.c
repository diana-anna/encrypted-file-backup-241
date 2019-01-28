#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char *str = strdup("ls");
  execlp(str, str, NULL);
}
