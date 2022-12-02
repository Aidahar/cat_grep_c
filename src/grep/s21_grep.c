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
  pattr *list = NULL;
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
        read_from_f(list, optarg);
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
  display(list);
  if (opt.e) {
    read_file(argc, optind, argv, &opt, list);
  } else {
    check_pattr(&list, argv[optind]);
    optind++;
    read_file(argc, optind, argv, &opt, list);
  }
  release(list);
  // release(files);
}

// читаем файл
void read_file(int argc, int ind, char **argv, struct options *opt,
               pattr *list) {
  int cnt_f = argc - ind;
  while (ind < argc) {
    FILE *fl = fopen(argv[ind], "r");
    if (fl) {
      read_lines(fl, opt, list, cnt_f, argv[ind]);
      fclose(fl);
    } else {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[ind]);
    }
    ind++;
  }
}

// читаем из файла паттернов в лист
void read_from_f(pattr *pat, char *file_name) {
  FILE *fl = fopen(file_name, "r");
  if (fl) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fl)) != -1) {
      check_pattr(&pat, line);
    }
    free(line);
    fclose(fl);
  } else {
    fprintf(stderr, "grep: %s: No such file or directory\n", file_name);
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

// читаем построчно файл
void read_lines(FILE *fl, struct options *opt, pattr *list, int cnt_files,
                char *file_name) {
  fseek(fl, 0, SEEK_SET);
  int cnt_if_c = 0, cnt_if_l = 0, cnt_line = 1, cnt_file_line = lines_file(fl);
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, fl)) != -1) {
    int count = compile_pattrn(opt, list, line);
    if (0 < count && !opt->c && !opt->l) {
      if (1 < cnt_files) {
        printf("%s:", file_name);
      }
      if (opt->n) {
        printf("%d:", cnt_line);
      }
      printf("%s", line);
    } else if (0 < count && (opt->c || opt->l)) {
      cnt_if_c++;
      cnt_if_l = 1;
      //} else if (0 < count && opt->l) {
    }
    print_last(count, cnt_line, cnt_file_line);
    cnt_line++;
  }
  if (opt->c && opt->l) {
    flag_c(cnt_files, cnt_if_l, opt, file_name);
    flag_l(cnt_files, cnt_if_l, opt, file_name);
  } else if (opt->c && !opt->l) {
    flag_c(cnt_files, cnt_if_c, opt, file_name);
  } else if (opt->l && !opt->c) {
    flag_l(cnt_files, cnt_if_l, opt, file_name);
  }
  free(line);
}

int compile_pattrn(struct options *opt, pattr *list, char *line) {
  int options = 0, cnt_p_in_l = 0, count = 0;
  int erroffset;
  const char *error;
  for (; list != NULL; list = list->next) {
    pcre *re;
    if (opt->i) {
      re = pcre_compile((char *)list->line, PCRE_CASELESS, &error, &erroffset,
                        NULL);
    } else {
      re = pcre_compile((char *)list->line, options, &error, &erroffset, NULL);
    }
    if (!re) {
      fprintf(stderr, "error pattrn");
    } else {
      count = 0;
      int ovector[30];
      count = pcre_exec(re, NULL, (char *)line, strlen(line), 0, PCRE_NOTEMPTY,
                        ovector, 30);
      if (0 < count) {
        cnt_p_in_l++;
      }
    }
    pcre_free((void *)re);
  }
  if (cnt_p_in_l && opt->v) {
    count = 0;
  } else if (!cnt_p_in_l && opt->v) {
    count = 1;
  } else if (cnt_p_in_l) {
    count = 1;
  } else {
    count = 0;
  }
  return count;
}

void flag_c(int cnt_files, int cnt_if_c, struct options *opt, char *file_name) {
  if (1 < cnt_files && opt->c) {
    printf("%s:%d\n", file_name, cnt_if_c);
  } else if (opt->c) {
    printf("%d\n", cnt_if_c);
  }
}

void flag_l(int cnt_files, int cnt_if_l, struct options *opt, char *file_name) {
  if (0 < cnt_if_l && opt->l) {
    printf("%s\n", file_name);
  }
  (void)cnt_files;
}

void print_last(int count, int cnt_line, int cnt_file_line) {
  if (0 < count && cnt_line == cnt_file_line) {
    printf("\n");
  }
}