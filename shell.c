#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

int main(void) {

    char *line = NULL;
    size_t size = 0;
    while (1)
    {
        char *args[64];
        int i = 0;

        printf("$ ");
        fflush(stdout);

        ssize_t len = getline(&line, &size, stdin);
        if(len == -1){
            printf("\n");
            break;
        }
        if (line[len - 1] == '\n'){
            line[len - 1] = '\0';
        }

        char *hold_word = strtok(line, " \t");
        if(hold_word == NULL){
            continue;
        }
        if (strcmp(hold_word, "exit") == 0){
            break;
        }
        while (hold_word != NULL && i < 63)
        {
            args[i] = hold_word;
            i++;
            hold_word = strtok(NULL, " ");
        }
        args[i] = NULL;   
        
        if(strcmp(args[0], "cd") == 0){
            if(args[1] == NULL){
                char *return_home = getenv("HOME");
                chdir(return_home);
            }else{
                if(chdir(args[1]) == -1){
                perror(args[1]);
                }
            }
            continue;
        }
        if(strcmp(args[0], "pwd") == 0){
            char path[4096];
            if (getcwd(path, sizeof(path)) == NULL){
                perror("pwd");
            }else{
                printf("%s\n", path);
            }    
            continue;
        }

        if(strcmp(args[0], "export") == 0){
            if(args[1] == NULL){
                int p = 0;
                while (environ[p] != NULL)
                {
                    printf("%s\n",environ[p]);
                    p++;
                }
                continue;
                
            } 
            char *eq = strchr(args[1], '=');
            if(eq == NULL){
                printf("Missing '='\n");
                continue;
            }
            *eq = '\0';
            setenv(args[1], eq + 1, 1);
            continue;
        }
        
        pid_t pid = fork();
        
        if(pid == 0){
            execvp(args[0], args);
            perror(args[0]);
            exit(1);
        }else{
            waitpid(pid , NULL, 0);
        }
                 
    }
    free(line);
    return 0;
}
