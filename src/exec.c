#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "exec.h"

volatile sig_atomic_t running = 0;

void run_pipe(char **args, char **cmd2){
  int fds[2];
  pipe(fds);

  running = 1;
  pid_t pid1 = fork();
  if (pid1 == 0) {
    signal(SIGINT, SIG_DFL);
    dup2(fds[1], 1);
    close(fds[0]);
    close(fds[1]);
    execvp(args[0], args);
    perror(args[0]);
    exit(1);
  }

  pid_t pid2 = fork();
  if (pid2 == 0) {
    signal(SIGINT, SIG_DFL);
    dup2(fds[0], 0);
    close(fds[0]);
    close(fds[1]);
    execvp(cmd2[0], cmd2);
    perror(cmd2[0]);
    exit(1);
  }

  close(fds[0]);
  close(fds[1]);
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
  running = 0;
}

void run_command(char **args, struct command *cmd){
  running = 1;
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    running = 0;
    return;
  }

  if (pid == 0) {
    signal(SIGINT, SIG_DFL);

    if(cmd->infile != NULL){
      int fo = open(cmd->infile, O_RDONLY);
      if(fo == -1){
        perror(cmd->infile);
        exit(1);
      }
      dup2(fo, 0);
      close(fo);
    }

    if(cmd->outfile != NULL){
      int fd = 0;
      if(cmd->append == 1){
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
      }else{
        fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      }
      if(fd == -1){
        perror(cmd->outfile);
        exit(1);
      }
      dup2(fd, 1);
      close(fd);
    }

    execvp(args[0], args);
    perror(args[0]);
    exit(1);
  }

  waitpid(pid, NULL, 0);
  running = 0;
}