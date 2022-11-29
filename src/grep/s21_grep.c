/*
  https://lloydrochester.com/post/c/regex_pcre/
  https://ru.wikipedia.org/wiki/PCRE
  https://github.com/PorterV/s21_SimpleBashUtils/blob/develop/src/grep/s21_grep.c
*/
#include "s21_grep.h"

int main(int argc, char **argv) {
  parse_flags(argc, argv);
  return 0;
}

// парсим флаги
void parse_flags(int argc, char **argv) {
  int ch = 0, long_opt = 0;
  struct options opt = {0};
  pattr *list = NULL, *files = NULL;
  char *short_options = ":c+e:+f:+h+i+l+n+o+t+v";
  static struct option long_options[] = {{NULL, 0, NULL, 0}};
  while (-1 != (ch = getopt_long(argc, argv, short_options, long_options,
                                 &long_opt))) {
    switch (ch) {
      case 'c':
        opt.c = 1;
        break;
      case 'e':  //требует паттерн поиска
        opt.e = 1;
        check_pattr(&list, optarg);
        break;
      case 'f':  //требуется файл
        opt.f = 1;
        check_pattr(&list, optarg);
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
        fprintf(stderr,
                "usage: grep [-cefhilnotv] [-e "
                "pattern] [-f file] [pattern] [file ...]");
        exit(1);
    }
  }
  if (opt.e) {
    read_file(argc, optind, argv, &opt, list);
  } else {
    check_pattr(&list, argv[optind]);
    optind++;
    read_file(argc, optind, argv, &opt, list);
  }
  // release(list);
  release(files);
}

// Временная функция для отображения флагов
void print_opt(struct options *opt) {
  printf("opt->c = %d\n", opt->c);
  printf("opt->e = %d\n", opt->e);
  printf("opt->f = %d\n", opt->f);
  printf("opt->h = %d\n", opt->h);
  printf("opt->i = %d\n", opt->i);
  printf("opt->l = %d\n", opt->l);
  printf("opt->n = %d\n", opt->n);
  printf("opt->o = %d\n", opt->o);
  printf("opt->t = %d\n", opt->t);
  printf("opt->v = %d\n", opt->v);
}

// Временная функция проверяющая флаг с
void print_field(int argc, struct options *opt) {
  if (opt->c) {
    while (argc >= optind) {
      printf("%s\n", optarg);
      optind++;
    }
  }
}

// читаем файл
void read_file(int argc, int ind, char **argv, struct options *opt,
               pattr *list) {
  while (ind < argc) {
    FILE *fl = fopen(argv[ind], "r");
    if (fl) {
      char *line = NULL;
      size_t len = 0, cnt_line = 0, cnt_line_file = lines_file(fl);
      ssize_t read;
      (void)cnt_line_file;
      fseek(fl, 0, SEEK_SET);
      pcre *re;
      int options = 0;
      const char *error;
      int erroffset;
      re = pcre_compile((char *)list->line, options, &error, &erroffset, NULL);
      if (!re) {
        fprintf(stderr, "error pattrn");
      } else {
        int count = 0;
        int ovector[30];
        display(list);
        while ((read = getline(&line, &len, fl)) != -1) {
          cnt_line++;
          count = pcre_exec(re, NULL, (char *)line, strlen(line), 0,
                            PCRE_NOTEMPTY, ovector, 30);
          if (count < 0) {
          } else {
            if (opt->n) {
              printf("%ld:%s", cnt_line, line);
            }
          }
        }
      }
      pcre_free((void *)re);
      free(line);
      fclose(fl);
    } else {
      fprintf(stderr, "grep: %s: No such file or directory", argv[ind]);
      exit(1);
    }
    (void)opt;
    (void)list;
    // display(list);
    ind++;
  }
}

// счетчик количества строк в файле
size_t lines_file(FILE *fl) {
  size_t cnt = 1;
  char ch = !EOF;
  while (EOF != ch) {
    if ('\n' == ch) cnt++;
    ch = fgetc(fl);
  }
  return cnt;
}

// проверяем список паттернов
void check_pattr(pattr **pat, char *list) {
  if (NULL == *pat) {
    *pat = create(list);
  } else {
    push_back(pat, list);
  }
}