#ifndef SRC_s21_grep_H_
#define SRC_s21_grep_H_
#define _POSIX_C_SOURCE 200809L

#include <getopt.h>
#include <pcre.h>
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

void check_pattr(pattr **pat, char *list);
int compile_pattrn(struct options *opt, pattr *list, char *line);
void flag_c(int cnt_files, int cnt_if_c, struct options *opt, char *file_name);
size_t lines_file(FILE *fl);  // счетчик количества строк в файле
void parse_flags(int argc, char **argv);
void print_opt(struct options *opt);  // временная функция для принта флагов
void read_file(int argc, int ind, char **argv, struct options *opt,
               pattr *list);
void read_lines(FILE *fl, struct options *opt, pattr *list, int cnt_files,
                char *file_name);
// void print_field(int argc, struct options *opt);  // временная функция

#endif  // SRC_s21_grep_H_
