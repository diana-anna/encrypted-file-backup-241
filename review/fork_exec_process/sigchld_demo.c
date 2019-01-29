#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LOG_AND_RUN(PROGRAM...)                                                \
  printf("line %d: "#PROGRAM"=%d\n", __LINE__, PROGRAM)

void handler(int sig) {
  (void)sig;
  int status;
  LOG_AND_RUN(waitpid(-1, &status, WNOHANG));
}

int main() {
  signal(SIGCHLD, handler);
  if (!fork()) {
    sleep(5);
    puts("Child is exiting");
    exit(0);
  }


  int status;
  LOG_AND_RUN(waitpid(-1, &status, WNOHANG));

  alarm(10);
  while (1) {
    puts("Parent is alive");
    sleep(1);
  }
}
