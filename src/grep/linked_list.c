#include "linked_list.h"

void create(pattr **pat, char *data) {
  pattr *tmp = (pattr *)malloc(sizeof(pattr));
  if (NULL == tmp) {
    fprintf(stderr, "cant allocate memory");
    exit(1);
  } else {
    tmp->line = strdup(data);
    tmp->next = NULL;
    (*pat) = tmp;
  }
}

void push_back(pattr *pat, char *data) {
  pattr *new = NULL;
  create(&new, data);
  pattr *cur = pat;
  while (NULL != cur->next) {
    cur = cur->next;
  }
  cur->next = new;
}

/* void print_list(pattr *pat) {
  while (NULL != pat) {
    printf("%s\n", pat->line);
    pat = pat->next;
  }
}
*/
void release(pattr *pat) {
  pattr *p = pat;
  while (NULL != p) {
    p = pat->next;
    free(pat->line);
    free(pat);
    pat = p;
  }
}
