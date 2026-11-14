#include <stdio.h>
#include <stdlib.h>


struct node {
    char *cmd;
    struct node *next;
};

int main(void) {
    
    char *cmds[] = {"ls", "pwd", "date", NULL};

    struct node *head = NULL;
    struct node *tail = NULL;

    for(int i = 0; cmds[i] != NULL; i++){
        struct node *n = malloc(sizeof(struct node));
        n->cmd = cmds[i];
        n->next = NULL;   

        if(head == NULL){
            head = n;
            tail = n;
        }else{
            tail->next = n;
            tail = n;
        }
    }
    struct node *p = head;
    while (p != NULL)
    {
        printf("%s\n", p->cmd);
        p = p->next;
    }
    p = head;
    while(p != NULL){
        struct node *next_one = p->next;
        free(p);
        p = next_one;
    }
    
    return 0;
}