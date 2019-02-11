/**
 * Shell Lab
 * CS 241 - Spring 2019
 */

#include "format.h"
#include "shell.h"
#include "vector.h"
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void view_command(const char* cwd, const char* command);
void exclamation(const char* cwd, const char* command);
void pound(const char* cwd, const char* command);
void cd(const char* cwd, const char* command);
int external_commands(const char* cwd, const char* command);

//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct process {
    char *command;
    pid_t pid;
} process;

//////////////////////////////////////////////////////////////////////////////////////////////
// external
extern char *optarg;
extern int optind;

//////////////////////////////////////////////////////////////////////////////////////////////
// globals
static vector* command_history;

//////////////////////////////////////////////////////////////////////////////////////////////
void fuck_everything() {
  // frees all

}

//////////////////////////////////////////////////////////////////////////////////////////////
void write_history_file(const char* filename) {
  if (access(filename, F_OK | R_OK | W_OK) == -1) { // file doesn't exist
    print_history_file_error();
    FILE* history_fs = fopen(filename, "w+"); // make new file with name 'filename'
    size_t i;
    for (i = 0; i < vector_size(command_history); i++) { // write all commands
      //char* curr = *vector_at(command_history, i);
      fprintf(history_fs,"%s", *vector_at(command_history, i));
      fprintf(history_fs, "\n");
    }
    fclose(history_fs);
    return;
  }
  //////////////////////////////////////////////////////////////////////////////////////////////

  // file exists, load in history file as history
  FILE* history_fs = fopen(filename, "r");
  char* buffer = NULL;
  size_t n = 0;

  vector_clear(command_history);

  while (1) {
    size_t command = getline(&buffer, &n, history_fs);
    if ((int)command == -1) break;
    buffer[command - 1] = 0; // get rid of \n
    vector_push_back(command_history, buffer);
  }
  fclose(history_fs);

}

//////////////////////////////////////////////////////////////////////////////////////////////
void commands_from_file(const char* filename) {
  if (access(filename, F_OK | R_OK) == -1) { // file doesn't exist
    print_script_file_error();
    return;
  }

  // file exists, open file and read commands line by line
  FILE* command_fs = fopen(filename, "r");
  char* buffer = NULL;
  size_t n = 0;

  while (1) {
    size_t command = getline(&buffer, &n, command_fs);
    if ((int)command == -1) break;
    buffer[command - 1] = 0; // get rid of \n

    char* buf = NULL;
    int n = 0;
    getcwd(buf, n);
    if (buf && command) {
      view_command(buf, buffer);
    } else {
      //fuck_everything();
      print_script_file_error();
      return;
    }

  }

}

//////////////////////////////////////////////////////////////////////////////////////////////
void view_command(const char* cwd, const char* command) {


  if (!cwd || !command) {
    print_script_file_error();
    return;
  }

  char* command_ = strdup(command);

  print_prompt(cwd, getpid());

  // built-in
  if (command_[0] == '!') {
    exclamation(cwd, command_);
    return;

  } else if (command_[0] == '#') {
    pound(cwd, command_);
    return;

  } else if (strncmp(command_, "cd",(size_t) 2)) {
    cd(cwd, command_);
    return;

//////////////////////////////////////////////////////////////////////////////////////////////

  } else { // external

    int logical = -1;

    // logicals
    if (strstr(command_, "&&")  != NULL) { // &&
      logical = 1;
    } else if (strstr(command_, "||")) { // ||
      logical = 2;
    } else if (strstr(command_, ";")) { // ;
      logical = 3;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////

    if (logical == -1) { // no bitwise
      print_prompt(cwd, getpid());
      print_command(command_);

      if (external_commands(cwd, command_) == -1) {
        print_exec_failed(command_);
        //free?
        return;
      }
      vector_push_back(command_history, command_);
      print_command_executed(getpid());
      //free?
      return;
//////////////////////////////////////////////////////////////////////////////////////////////
    } else if (logical == 1) { // &&
      // TODO
      print_prompt(cwd, getpid());
      print_command(command_);

      size_t i;
      int count = 0;
      for (i = 0; i < strlen(command_) - 1; i++) {
        if (command_[i] == '&' && command_[i + 1] == '&') count++;
      }

      char** args = malloc((count + 1) * sizeof(char*) + 1);
      char* token;
      token = strtok(command_, "&&");
      i = 0;
      while (token) {
        args[i] = malloc(strlen(token)); // command & its space = 'token'
        args[i] = token;
        args[i][strlen(token)] = 0; // get rid of its space and just get command
        i++;
        token = strtok(NULL, "&&");
      }

      args[i] = 0; // terminate args w null

      for (i = 0; i < (size_t) (count + 1); i++) {
        if (external_commands(cwd, args[i]) == -1) {
          print_exec_failed(args[i]);
          return;
        }
      }

      print_command_executed(getpid());
      //free?
      return;
      //////////////////////////////////////////////////////////////////////////////////////////////
    } else if (logical == 2) { // ||
      // TODO

      print_prompt(cwd, getpid());
      print_command(command_);

      size_t i;
      int count = 0;
      for (i = 0; i < strlen(command_) - 1; i++) {
        if (command_[i] == '|' && command_[i + 1] == '|') count++;
      }

      char** args = malloc((count + 1) * sizeof(char*) + 1);
      char* token;
      token = strtok(command_, "||");
      i = 0;
      while (token) {
        args[i] = malloc(strlen(token)); // command & its space = 'token'
        args[i] = token;
        args[i][strlen(token)] = 0; // get rid of its space and just get command
        i++;
        token = strtok(NULL, "||");
      }

      args[i] = 0; // terminate args w null

      for (i = 0; i < (size_t) (count + 1); i++) {
        if (external_commands(cwd, args[i]) != -1) { // one works
          print_command_executed(getpid());
          //free?
          return;
        }
      }
      print_exec_failed(command_); // every single one failed
      //free?
      return;
      //////////////////////////////////////////////////////////////////////////////////////////////
    } else if (logical == 3) { // ;
      // TODO
      print_prompt(cwd, getpid());
      print_command(command_);

      size_t i;
      int count = 0;
      for (i = 0; i < strlen(command_); i++) {
        if (command_[i] == ';') count++;
      }

      char** args = malloc((count + 1) * sizeof(char*) + 1);
      char* token;
      token = strtok(command_, ";");
      i = 0;
      while (token) {
        args[i] = malloc(strlen(token)); // command & its space = 'token'
        args[i] = token;
        args[i][strlen(token)] = 0; // get rid of its space and just get command
        i++;
        token = strtok(NULL, ";");
      }

      args[i] = 0; // terminate args w null

      for (i = 0; i < (size_t) (count + 1); i++) { // just run commands one by one
        if (external_commands(cwd, args[i]) == -1) {
          print_exec_failed(args[i]);
        } else {
          print_command_executed(getpid());
        }
      }

      //free?
      return;

    }
//////////////////////////////////////////////////////////////////////////////////////////////

  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void pound(const char* cwd, const char* command) {
  if(strlen(command) < 2) {
    print_invalid_index();
    return;
  }

  size_t i;

  for (i = 0; i < strlen(&command[1]); i++) {
    // check that all numbers
    if (!isdigit(command[i])) {
      print_invalid_index();
      return;
    }
  }

  char* ptr;
  long command_num = strtol(&command[1], &ptr, 10);
  if (command_num < 0 || (size_t) command_num > vector_size(command_history) - 1) {
    print_invalid_index();
    return;
  }

  char* execute_me = *vector_at(command_history, command_num);
  print_command(execute_me);
  view_command(cwd, execute_me);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void exclamation(const char* cwd, const char* command) {
  if (!strcmp(command, "!history")) { // NOT STORED IN HISTORY VECTOR
    size_t i;
    for (i = 0; i < vector_size(command_history); i++) {
      print_history_line(i, *vector_at(command_history, i));
    }
    return;
  }

  if (strlen(command) == 1) { // exec last command
    char* last_command = *vector_at(command_history, vector_size(command_history) - 1);
    vector_push_back(command_history, last_command);
    view_command(cwd, last_command);
    return;
  }
  if (strlen(command) > 1) { // search for search_term
    const char* search_term = &command[1];
    size_t i;
    //int found = 0;
    for (i = 0; i < vector_size(command_history); i++) {
      char* execute_me = *vector_at(command_history, i);
      if (!strncmp(search_term, execute_me, strlen(search_term))) {
        //found = 1;
        view_command(cwd, execute_me);
        return;
      }
    }
    print_no_history_match();
    return;

  }

}

//////////////////////////////////////////////////////////////////////////////////////////////
void cd(const char* cwd, const char* command) {
  if (strlen(command) < 4) {
    print_no_directory(&command[0]);
    return;
  }

  if (chdir(&command[3]) != -1) {
    print_no_directory(&command[3]);
    return;
  }
  //vector_push_back(command_history, command);
}

//////////////////////////////////////////////////////////////////////////////////////////////
int external_commands(const char* cwd, const char* command) {

  int backgrounding = 0;
  char* command_;

  pid_t child_id = fork();

  if (child_id < 0) {
    print_fork_failed();
    return -1;
  }

//////////////////////////////////////////////////////////////////////////////////////////////
  if (child_id == 0) { // CHILD

    if (command[strlen(command) - 1] == '&') {
      backgrounding = 1;
      command_ = strndup(command, strlen(command - 1)); // get command without '&'
      //free this^^^^^^

    } else {
      command_ = strdup(command); // free!!!!!
    }

    // count # times space occurs
    size_t i;
    int count = 0;
    for (i = 0; i < strlen(command_); i++) {
      if (command_[i] == ' ') count++;
    }

    char** args = malloc((count + 1) * sizeof(char*) + 1);
    char* token;
    token = strtok(command_, " ");
    i = 0;
    while (token) {
      args[i] = malloc(strlen(token) + 1);
      args[i] = token;
      args[i][strlen(token)] = 0;
      i++;
      token = strtok(NULL, " ");
    }

    args[i] = 0; // terminate args w null
    execvp(args[0], args);
    print_exec_failed(command);
    free(command_);
    size_t j;
    for (j = 0; j < (size_t) (count + 2); j++) {
      free(args[j]);
    }
    free(args);
    //free?
    return -1;

  } else {

//////////////////////////////////////////////////////////////////////////////////////////////
  // PARENT
    if (backgrounding) {
      return 0;
    } else {
      if (setpgid(child_id, child_id) == -1) {
        print_setpgid_failed();
        exit(1);
      }

      int status;
      waitpid(child_id, &status, 0);

      if (!WIFEXITED(status)) { // child terminated abnormally
        print_wait_failed();
        // free?
        return -1;
      }

      return 0; // child terminated normally
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////

void signal_handler(int signal) {
  switch(signal) {
    case SIGINT:
      break;
    case SIGCHLD:
      while (waitpid((pid_t)(-1), 0, WNOHANG) > 0);
      break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
int shell(int argc, char *argv[]) {
    // TODO: This is the entry point for your shell.

    // signals
    signal(SIGINT, signal_handler);
    signal(SIGCHLD, signal_handler);

    if (argc > 5 || argc % 2 == 0) {
      print_usage();
    }

    // check for option
    int h_flag = 0;
    int f_flag = 0;
    char* h_arg = NULL;
    char* f_arg = NULL;
    char opt;
    command_history = vector_create(char_copy_constructor, char_destructor,
      char_default_constructor); // free ?

//////////////////////////////////////////////////////////////////////////////////////////////
    while ((opt = getopt(argc, argv, "h:f:")) != -1) {
      switch(opt) {
        case 'h':
          h_flag = 1;
		      h_arg = strdup(optarg);
		      break;
	      case 'f':
		      f_flag = 1;
		      f_arg = strdup(optarg);
		      break;
	      case '?':
          print_usage();
          break;
	     }
    }

//////////////////////////////////////////////////////////////////////////////////////////////
    if (h_flag) {
      write_history_file(h_arg);
    }

    if (f_flag) {
      commands_from_file(f_arg);
    }

    free(h_arg);
    free(f_arg);

    return 0;
}

