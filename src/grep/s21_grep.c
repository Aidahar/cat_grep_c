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
  char *short_options = ":c+e:+f:+h+i+l+n+o+t+v";
  static struct option long_options[] = {{NULL, 0, NULL, 0}};
  pattr *list = NULL;
  pattr *files = NULL;
  while (-1 != (ch = getopt_long(argc, argv, short_options, long_options,
                                 &long_opt))) {
    switch (ch) {
      case 'c':
        opt.c = 1;
        break;
      case 'e':  //требует паттерн поиска
        opt.e = 1;
        if (NULL == list) {
          list = create(optarg);
        } else {
          push_back(&list, optarg);
        }
        break;
      case 'f':  //требуется файл
        opt.f = 1;
        if (NULL == files) {
          files = create(optarg);
        } else {
          push_back(&files, optarg);
        }
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
    while (optind < argc) {
      read_file(argv[optind], &opt, list);
      optind++;
    }
  } else {
    if (NULL == list) {
      list = create(argv[optind]);
    } else {
      push_back(&list, argv[optind]);
    }
    optind++;
    while (optind < argc) {
      read_file(argv[optind], &opt, list);
      optind++;
    }
  }
  release(list);
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
void read_file(char *file_name, struct options *opt, pattr *list) {
  FILE *fl = fopen(file_name, "r");
  if (fl) {
    char *line = NULL;
    size_t len, cnt_line, cnt_line_file;
    len = cnt_line = cnt_line_file = 0;
    cnt_line_file = lines_file(fl);
    ssize_t read;
    fseek(fl, 0, SEEK_SET);
    while ((read = getline(&line, &len, fl)) != -1) {
      printf("%s", line);
      cnt_line++;
      if (opt->c) {
      }
    }
    free(line);
    fclose(fl);
  } else {
    fprintf(stderr, "grep: %s: No such file or directory", file_name);
    exit(1);
  }
  (void)list;
}

// счетчик количества строк в файле
size_t lines_file(FILE *fl) {
  size_t cnt = 1;
  char ch = fgetc(fl);
  while (EOF != ch) {
    if ('\n' == ch) cnt++;
    ch = fgetc(fl);
  }
  return cnt;
}
