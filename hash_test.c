#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct alias
{
    char *name;
    char *value;
    struct alias *next;
};

struct alias *table[15] = {NULL};

int hash(char *name){
    int sum = 0;
    for (int p = 0; name[p] != '\0'; p++){
        sum += name[p];
    }
    return sum % 15;
}

void add_alias(char *name, char *value){
    int index = hash(name);

    struct alias *p = table[index];
    while (p != NULL) {
        if (strcmp(p->name, name) == 0) {
            free(p->value);
            p->value = strdup(value);
            return;
        }
        p = p->next;
    }

    struct alias *n = malloc(sizeof(struct alias));
    n->name = strdup(name);
    n->value = strdup(value);
    n->next = table[index];
    table[index] = n;
}

char *find_alias(char *name){
    int index = hash(name);
    struct alias *p = table[index];

    while (p != NULL)
    {
        if (strcmp(p->name, name) == 0) {
            return p->value;
        }
        p = p->next;
    }
    return NULL;
    
}

void free_table(void){
    for (int i = 0; i < 15; i++) {
        struct alias *p = table[i];

        while (p != NULL) {
            struct alias *next_one = p->next;
            free(p->name);
            free(p->value);
            free(p);
            p = next_one;
        }

        table[i] = NULL;
    }
}

int main(){

    add_alias("ll", "ls -l");
    add_alias("la", "ls -a");
    add_alias("al", "ls -al");
    add_alias("ll", "ls -lh");
    printf("%s\n", find_alias("ll"));

    printf("%s\n", find_alias("ll"));
    printf("%s\n", find_alias("al"));
    printf("%s\n", find_alias("la"));

    // char *result = find_alias("xy");

    // if (result == NULL) {
    //     printf("not found\n");
    // }else {
    //     printf("%s\n", result);
    // }
    free_table();
}
