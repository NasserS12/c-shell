#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "history.h"
#include "builtins.h"
#include "parse.h"
#include "exec.h"


void handle_sigint(int sig) {
  if(running == 1){
    write(1, "\n", 1);
  }else{
    write(1, "\n$ ", 3);
  }
}


int main(void){
  struct sigaction sa = {0};
  sa.sa_handler = handle_sigint;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGINT, &sa, NULL);

  char *line = NULL;
  size_t size = 0;

  while (1) {
    char *args[64];

    printf("$ ");
    fflush(stdout);

    ssize_t len = getline(&line, &size, stdin);
    if (len == -1) {
      printf("\n");
      break;
    }
    if (line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }

    add_history(line);

    int i = parse_line(line, args);
    if(i == 0){
      continue;
    }
    if(strcmp(args[0], "exit") == 0){
      break;
    }

    if (strcmp(args[0], "cd") == 0) {
      do_cd(args);
      continue;
    }

    if (strcmp(args[0], "pwd") == 0) {
      do_pwd();
      continue;
    }

    if(strcmp(args[0], "history") == 0){
      print_history();
      continue;
    }

    if (strcmp(args[0], "export") == 0) {
      do_export(args);
      continue;
    }

    struct command cmd = {0};
    int bad = parse_redirects(args, i, &cmd);
    if(bad == 1){
      continue;
    }

    if (cmd.cmd2 != NULL){
      run_pipe(args, cmd.cmd2);
      continue;
    }

    run_command(args, &cmd);
  }
  free(line);
  free_history();
  return 0;
}