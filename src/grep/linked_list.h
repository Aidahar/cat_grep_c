#ifndef SRC_linked_list_H_
#define SRC_linked_list_H_
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pattr {
  char *line;
  struct pattr *next;
} pattr;

void create(pattr **pat, char *data);
// void print_list(pattr *start);
void push_back(pattr *list, char *data);
void release(pattr *list);

#endif  // SRC_linked_list_H_
