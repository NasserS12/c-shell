#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

extern char **environ;

int main(void) {

  char *line = NULL;
  size_t size = 0;
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
    char *outfile = NULL;
    for(int k = 0; args[k] != NULL; k++){
        if(strcmp(args[k], ">") == 0){
            outfile = args[k + 1];
            args[k] =  NULL;
            break;
        }
    }

    pid_t pid = fork();
    if (pid == -1) {
      perror("fork");
      continue;
    }

    if (pid == 0) {
        if(outfile != NULL){
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
    }
  }
  free(line);
  return 0;
}
