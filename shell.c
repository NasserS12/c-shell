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
        if(*line == '\0'){
            continue;
        }

        if (strcmp(line, "exit") == 0){
            break;
        }
        printf("you write : %s\n",line);
    }
    free(line);
    return 0;
}
