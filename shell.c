#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("$ ");
    fflush(stdout);

    char *line = NULL;
    size_t size = 0;
    ssize_t len = getline(&line, &size, stdin);
    if(len == -1){
        printf("\n");
        free(line);
        return 0;
    }
    printf("you write : %s",line);
    free(line);
    return 0;
}