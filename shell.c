#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

    char *line = NULL;
    size_t size = 0;
    while (1)
    {
        char *args[64];
        int i = 0;
        int j = 0;
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
            hold_word = strtok(NULL, " \t");
        }
        args[i] = NULL;   
        while (args[j] != NULL)
        {
            if (j > 0) printf(" ");
            printf("%s",args[j]);
            j++;
        }
        printf("\n");
        
                 
    }
    free(line);
    return 0;
}
