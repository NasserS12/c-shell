#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

struct node {
    char *cmd;
    struct node *next;
};

static
struct node *head = NULL;
static
struct node *tail = NULL;

void add_history(char *line){

  struct node *n = malloc(sizeof(struct node));
  n->cmd = strdup(line);
  n->next = NULL;

  if(head == NULL){
      head = n;
      tail = n;
  }else{
    tail->next = n;
    tail = n;
  }

}

void print_history(void) {
    struct node *p = head;
    while (p != NULL) {
        printf("%s\n", p->cmd);
        p = p->next;
    }
}

void free_history(void){
    struct node *p = head;
    while(p != NULL){
      struct node *next_one = p->next;
      free(p->cmd);
      free(p);
      p = next_one;
  }
}