/*
 * =====================================================================================
 *
 *       Filename:  try_i.c
 *
 *    Description: try pcre CASSELLESS 
 *
 *        Version:  1.0
 *        Created:  12/02/2022 09:36:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pcre.h>

int main(int argc, char **argv) {
  (void)argc;
  char *pattr_string = "HelaGa";
  char *pat = "";
  if (1 < argc) {
    pat = argv[1];
  }
  printf("pat = %s\n", pat);
  int erroffset;
  const char *error;
  pcre *re;
  int res = 0;
  re = pcre_compile((char *)pat, PCRE_CASELESS, &error, &erroffset, NULL);
  if (!re) {
    fprintf(stderr, "error pattern");
    exit(1);
  } else {
    int ovector[30];
    res = pcre_exec(re, NULL, (char *)pattr_string, strlen(pattr_string), 0, PCRE_NOTEMPTY, ovector, 30);
    printf("res = %d\n", res);
  }
  if (0 < res) {
    printf("pattr_string = %s\n", pattr_string);
  }
  pcre_free((void *)re);
  return 0;
}

