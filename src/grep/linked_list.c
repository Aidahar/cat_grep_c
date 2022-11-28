#include "linked_list.h"

pattr *create(char *lt) {
  pattr *list = malloc(sizeof(pattr));
  list->line = strdup(lt);
  list->next = NULL;
  return list;
}

void display(pattr *start) {
  pattr *current = start;
  for (; current != NULL; current = current->next) {
    printf("line = %s\n", current->line);
  }
}

void push_back(pattr **list, char *pat) {
  pattr *new_elem = NULL;
  new_elem = create(pat);
  pattr *tmp = *list;
  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  tmp->next = new_elem;
}


void release(pattr *list) {
  pattr *old = list;
  pattr *next = NULL;
  for (; old != NULL; old = next) {
    next = old->next;
    free(old->line);
    free(old);
  }
}
