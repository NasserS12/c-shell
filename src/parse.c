#include <stdio.h>
#include <string.h>
#include "parse.h"

int parse_line(char *line, char **args){
  int i = 0;

  char *hold_word = strtok(line, " \t");
  if (hold_word == NULL) {
      return 0;
  }

  while (hold_word != NULL && i < 63) {
      args[i] = hold_word;
      i++;
      hold_word = strtok(NULL, " \t");
  }
  args[i] = NULL;
  return i;
}

int parse_redirects(char **args, int i, struct command *cmd){

  for(int k = 0; k < i; k++){
    if(strcmp(args[k], ">") == 0){
      if(args[k + 1] == NULL){
        printf("syntax error: missing file name\n");
        return 1;
      }
      cmd->outfile = args[k + 1];
      args[k] = NULL;
    }else if (strcmp(args[k], "<") == 0){
      if(args[k + 1] == NULL){
        printf("syntax error: missing file name\n");
        return 1;
      }
      cmd->infile = args[k + 1];
      args[k] = NULL;
    }else if (strcmp(args[k], ">>") == 0){
      if(args[k + 1] == NULL){
        printf("syntax error: missing file name\n");
        return 1;
      }
      cmd->append = 1;
      cmd->outfile = args[k + 1];
      args[k] = NULL;
    }else if (strcmp(args[k], "|") == 0){
      if(args[k + 1] == NULL){
        printf("syntax error: missing command after '|'\n");
        return 1;
      }
      cmd->cmd2 = &args[k + 1];
      args[k] = NULL;
    }
  }
  return 0;
}