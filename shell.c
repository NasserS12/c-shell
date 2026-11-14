#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

extern char **environ;
volatile sig_atomic_t running = 0;
struct node {
    char *cmd;
    struct node *next;
};

void handle_sigint(int sig) {
  if(running == 1){
    write(1, "\n", 1);
  }else{
    write(1, "\n$ ", 3);
  }
}

int main(void) {
  struct sigaction sa = {0};
  sa.sa_handler = handle_sigint;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGINT, &sa, NULL);

  char *line = NULL;
  size_t size = 0;

  struct node *head = NULL;
  struct node *tail = NULL;

  while (1) {
    char *args[64];
    int i = 0;

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

    struct node *n = malloc(sizeof(struct node));
    n->cmd = strdup(line);
    n->next = NULL;

    if(head == NULL){
        head = n;
        tail = n;
    }else{

      tail->next = n;
      tail = n;
    }
    

    char *hold_word = strtok(line, " \t");
    if (hold_word == NULL) {
      continue;
    }
    if (strcmp(hold_word, "exit") == 0) {
      break;
    }
    while (hold_word != NULL && i < 63) {
      args[i] = hold_word;
      i++;
      hold_word = strtok(NULL, " \t");
    }
    args[i] = NULL;

    if (strcmp(args[0], "cd") == 0) {
      if (args[1] == NULL) {
        char *return_home = getenv("HOME");
        chdir(return_home);
      } else {
        if (chdir(args[1]) == -1) {
          perror(args[1]);
        }
      }
      continue;
    }
    if (strcmp(args[0], "pwd") == 0) {

      char path[4096];
      if (getcwd(path, sizeof(path)) == NULL) {
        perror("pwd");
      } else {
        printf("%s\n", path);
      }
      continue;
    }

    if(strcmp(args[0], "history") == 0){
      struct node *p = head;
      while (p != NULL)
      {
        printf("%s\n",p->cmd);
        p = p->next;
      }
      continue;
    }

    if (strcmp(args[0], "export") == 0) {
      if (args[1] == NULL) {
        int p = 0;
        while (environ[p] != NULL) {
          printf("%s\n", environ[p]);
          p++;
        }
        continue;
      }
      char *eq = strchr(args[1], '=');
      if (eq == NULL) {
        printf("Missing '='\n");
        continue;
      }
      *eq = '\0';
      setenv(args[1], eq + 1, 1);
      continue;
    }

    int bad = 0;
    int append = 0;
    char *infile = NULL;
    char *outfile = NULL;
    char **cmd2 = NULL;

    for(int k = 0; k < i; k++){
        if(strcmp(args[k], ">") == 0){
          if(args[k + 1] == NULL){
            printf("syntax error: missing file name\n");
            bad = 1;
            break;
          }
          outfile = args[k + 1];
          args[k] =  NULL;
        }else if (strcmp(args[k], "<") == 0){
            if(args[k + 1] == NULL){
              printf("syntax error: missing file name\n");
              bad = 1;
              break;
            }
          infile = args[k + 1];
          args[k] = NULL;
        }else if (strcmp(args[k], ">>") == 0){
            if(args[k + 1] == NULL){
            printf("syntax error: missing file name\n");
            bad = 1;
            break;
          }
          append = 1;
          outfile = args[k + 1];
          args[k] = NULL;
        }else if (strcmp(args[k], "|") == 0){
          if(args[k + 1] == NULL){
            printf("syntax error: missing command after '|'\n");
            bad = 1;
            break;
          }
          cmd2 = &args[k + 1];
          args[k] = NULL;
        }
  }


      if(bad == 1){
        continue;
    }
    
    if (cmd2 != NULL){

      int fds[2];
      pipe(fds);

      running = 1;
      pid_t pid1 = fork();
      if (pid1 == 0) {
        signal(SIGINT, SIG_DFL);
        dup2(fds[1],1);

        close(fds[0]);
        close(fds[1]);
        execvp(args[0], args);
        perror(args[0]);
        exit(1);
      }

      pid_t pid2 = fork();
      if (pid2 == 0) {
        signal(SIGINT, SIG_DFL);
        dup2(fds[0],0);

        close(fds[0]);
        close(fds[1]);
        execvp(cmd2[0],cmd2);
        perror(cmd2[0]);
        exit(1);
      }

      close(fds[0]);
      close(fds[1]);
      waitpid(pid1, NULL, 0);
      waitpid(pid2, NULL, 0);
      running = 0;

      continue;
    }

    running = 1;
    pid_t pid = fork();
    if (pid == -1) {
      perror("fork");
      running = 0;
      continue;
    }

    if (pid == 0) {
      signal(SIGINT, SIG_DFL);

      if(infile != NULL){
        int fo = open(infile, O_RDONLY);

        if(fo == -1){
          perror(infile);
          exit(1);
        }
        dup2(fo,0);
        close(fo);
      }
        if(outfile != NULL){
          int fd = 0;

          if(append == 1){
            fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
          }else{
            fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
          }
          if(fd == -1){
              perror(outfile);
              exit(1);
          }
          
          dup2(fd, 1);
          close(fd);
        }
      execvp(args[0], args);
      perror(args[0]);
      exit(1);
    } else {
      waitpid(pid, NULL, 0);
      running = 0;
    }
  }
  free(line);
  struct node *p = head;
  while(p != NULL){
    struct node *next_one = p->next;
    free(p->cmd);
    free(p);
    p = next_one;
  }
  return 0;
}
