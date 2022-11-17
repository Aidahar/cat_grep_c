#include "s21_grep.h"

int main(int argc, char **argv) {
  parse_flags(argc, argv);
  return 0;
}

void parse_flags(int argc, char **argv) {
  struct options opt = {0};
  int ch = 0;
  char *short_opt = "ce:f:hilnotv";
  while (-1 != (ch = getopt(argc, argv, short_opt))) {
    switch (ch) {
      case 'c':
        opt.c = 1;
        break;
      case 'e':  //требует паттерн поиска
        opt.e = 1;
        break;
      case 'f':  //требуется файл
        opt.f = 1;
        break;
      case 'h':
        opt.h = 1;
        break;
      case 'i':
        opt.i = 1;
        break;
      case 'l':
        opt.l = 1;
        break;
      case 'n':
        opt.n = 1;
        break;
      case 'o':
        opt.o = 1;
        break;
      case 't':
        opt.t = 1;
        break;
      case 'v':
        opt.v = 1;
        break;
      default:
        fprintf(
            stderr,
            "grep [-cefhilnotv] [-e pattern] [-f file] [pattern] [file ...]");
        exit(1);
    }
  }
  print_field(&opt);
}

void print_field(struct options *opt) {
  printf("FIELDS FLAGS: \n");
  printf("opt->c: %d\n", opt->c);
  printf("opt->e: %d\n", opt->e);
  printf("opt->f: %d\n", opt->f);
  printf("opt->h: %d\n", opt->h);
  printf("opt->i: %d\n", opt->i);
  printf("opt->l: %d\n", opt->l);
  printf("opt->n: %d\n", opt->n);
  printf("opt->o: %d\n", opt->o);
  printf("opt->t: %d\n", opt->t);
  printf("opt->v: %d\n", opt->v);
}