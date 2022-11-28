#ifndef SRC_s21_grep_H_
#define SRC_s21_grep_H_
#define _POSIX_C_SOURCE 200809L

#include <getopt.h>
#include <pcre.h>
// #include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

struct options {
  unsigned char c : 1;
  unsigned char e : 1;
  unsigned char f : 1;
  unsigned char h : 1;
  unsigned char i : 1;
  unsigned char l : 1;
  unsigned char n : 1;
  unsigned char o : 1;
  unsigned char t : 1;
  unsigned char v : 1;
};

void parse_flags(int argc, char **argv);
void print_field(int argc, struct options *opt);
void read_file(char *file_name, struct options *opt, pattr *list);
void print_opt(struct options *opt);  // временная функция для принта флагов
size_t lines_file(FILE *fl);  // временная функция для печати файла построчно

#endif  // SRC_s21_grep_H_
