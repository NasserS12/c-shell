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
        printf("you write : %s",line);
    }
    free(line);
    return 0;
}