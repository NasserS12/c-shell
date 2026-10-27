#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

    char *line = NULL;
    size_t size = 0;
    while (1)
    {
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

        char *hold_word = strtok(line, " ");
        if(hold_word == NULL){
            continue;
        }
        if (strcmp(hold_word, "exit") == 0){
            break;
        }
        while (hold_word != NULL)
        {
            printf("word: %s\n",hold_word);
            hold_word = strtok(NULL, " ");
        }                
    }
    free(line);
    return 0;
}
