#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define str(x...) xstr(x)
#define xstr(x...) #x
#define LOG_AND_RUN(PROGRAM...)                                                \
  printf("line "str(__LINE__)": "str(PROGRAM)"=%d\n", PROGRAM)

void handler(int sig) {
  (void)sig;
  int status;
  LOG_AND_RUN(waitpid(-1, &status, WNOHANG));
}

int main() {
  signal(SIGCHLD, handler);
  pid_t child;
  if (!(child = fork())) {
    sleep(5);
    puts("Child is exiting");
    exit(0);
  } else {
    printf("Child pid is %d\n", child);
  }


  int status;
  LOG_AND_RUN(waitpid(-1, &status, WNOHANG));

  alarm(10);
  while (1) {
    puts("  Parent is alive");
    sleep(1);
  }
}
