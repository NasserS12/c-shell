#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alias.h"

struct alias
{
    char *name;
    char *value;
    struct alias *next;
};

static
struct alias *table[15] = {NULL};

static
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
