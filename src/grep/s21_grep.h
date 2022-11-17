#ifndef SRC_s21_grep_H_
#define SRC_s21_grep_H_
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

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
void print_field(struct options *opt);

#endif  // SRC_s21_cat_H_