#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pattr {
  char *line;
  struct pattr *next;
} pattr;

pattr *create(char *pat);
void display(pattr *start);
void push_back(pattr **list, char *pat);
void release(pattr *list);
