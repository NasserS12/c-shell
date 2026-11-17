#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtins.h"
#include "alias.h"

extern char **environ;


void do_pwd(void) {
  char path[4096];
  if (getcwd(path, sizeof(path)) == NULL){
    perror("pwd");
  }else{
    printf("%s\n", path);
  }
}

void do_cd(char **args){
	if(args[1] == NULL) {
		char *return_home = getenv("HOME");
		chdir(return_home);
	}else{
		if(chdir(args[1]) == -1){
			perror(args[1]);
		}
	}
}

void do_export(char **args){
	if(args[1] == NULL){
		int p = 0;
		while (environ[p] != NULL){
			printf("%s\n", environ[p]);
			p++;
		}
		return;
	}
	char *eq = strchr(args[1], '=');
	if(eq == NULL){
		printf("Missing '='\n");
		return;
	}
	*eq = '\0';
	setenv(args[1], eq + 1, 1);
}

void do_alias(char **args){
	char *eq = strchr(args[1], '=');
	if(eq == NULL){
		printf("Missing '='\n");
		return;
	}
	*eq = '\0';
	add_alias(args[1], eq + 1);
}