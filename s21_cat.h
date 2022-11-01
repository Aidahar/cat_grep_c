#ifndef SRC_s21_cat_H_
#define SRC_s21_cat_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cnt_symbol(char *str);
static int file_open(const char *file_name);
static void read_line(FILE *file);
static void print_b(int *c, const char *line);
static void print_e(char *line);
static void print_n(int c, const char *line);
static void print_t(char *line);

#endif  // SRC_s21_cat_H_
