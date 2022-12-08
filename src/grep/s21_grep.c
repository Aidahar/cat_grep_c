#include "s21_grep.h"

int main(int argc, char **argv) {
  if (2 < argc) {
    parse_flags(argc, argv);
  }
  return 0;
}

// проверяем список паттернов
void check_pattr(pattr **pat, char *data) {
  if (NULL == *pat) {
    create(pat, data);
  } else {
    push_back(*pat, data);
  }
}

// проверяем на наличие паттерна в строке
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
      if (!opt->s) {
        fprintf(stderr, "error pattrn\n");
      }
    } else {
      count = 0;
      int ovector[30];
      count =
          pcre_exec(re, NULL, (char *)line, strlen(line), 0, 0, ovector, 30);
      if (0 < count) {
        cnt_p_in_l++;
      }
      flag_o(re, count, opt, ovector, line);
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
  (1 < cnt_files && opt->c && !opt->h) ? printf("%s:%d\n", file_name, cnt_if_c)
                                       : printf("%d\n", cnt_if_c);
}

void flag_l(int cnt_if_l, struct options *opt, char *file_name) {
  if (0 < cnt_if_l && opt->l) printf("%s\n", file_name);
}

void flag_o(pcre *re, int count, struct options *opt, int *ovector,
            char *line) {
  int idx_s = ovector[0];
  int idx_end = ovector[1];
  if (0 < count && opt->o && !opt->v && !opt->c) {
    while (0 < count) {
      for (; idx_s < idx_end; idx_s++) {
        printf("%c", line[idx_s]);
      }
      printf("%c", '\n');
      count = pcre_exec(re, NULL, (char *)line, strlen(line), idx_end, 0,
                        ovector, 30);
      idx_s = ovector[0];
      idx_end = ovector[1];
    }
  }
}

// счетчик количества строк в файле
size_t lines_file(FILE *fl) {
  size_t cnt = 1;
  char ch = !EOF;
  while (EOF != ch) {
    ch = fgetc(fl);
    if ('\n' == ch) cnt++;
  }
  return cnt;
}

// парсим флаги
void parse_flags(int argc, char **argv) {
  int ch = 0, long_opt = 0;
  struct options opt = {0};
  pattr *list = NULL;
  pattr *files = NULL;
  char *short_options = ":c+e:+h+i+l+n+o+s+v+f:";
  static struct option long_options[] = {{NULL, 0, NULL, 0}};
  while (-1 != (ch = getopt_long(argc, argv, short_options, long_options,
                                 &long_opt))) {
    switch (ch) {
      case 'c':
        opt.c = 1;
        break;
      case 'e':  // требует паттерн поиска
        opt.e = 1;
        check_pattr(&list, optarg);
        break;
      case 'f':  // требуется файл
        opt.f = 1;
        check_pattr(&files, optarg);
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
      case 's':
        opt.s = 1;
        break;
      case 'v':
        opt.v = 1;
        break;
      default:
        fprintf(stderr,
                "usage: grep [-cefhilnosv] [-e "
                "pattern] [-f file] [pattern] [file ...]");
        exit(1);
    }
  }
  if (opt.f) {
    read_from_f(&list, files);
  }
  if (opt.e || opt.f) {
    read_file(argc, optind, argv, &opt, list);
  } else {
    check_pattr(&list, argv[optind]);
    optind++;
    read_file(argc, optind, argv, &opt, list);
  }
  release(list);
}

void print_last(int count, int cnt_line, int cnt_file_line) {
  if (0 < count && cnt_line == cnt_file_line) printf("\n");
}

// читаем из файла паттернов в лист
void read_from_f(pattr **pat, pattr *files) {
  while (files) {
    if (255 < strlen(files->line)) {
      fprintf(stderr, "grep: %s: File name too long\n", files->line);
      exit(1);
    } else {
      FILE *fl = fopen(files->line, "r");
      if (fl) {
        size_t len = 0;
        ssize_t read;
        char *l = NULL, *patr = NULL;
        while (-1 != (read = getdelim(&l, &len, '\n', fl))) {
          patr = strdup(l);
          if (patr[read - 1] == '\n') {
            patr[read - 1] = '\0';
          }
          check_pattr(pat, patr);
        }
        free(l);
        fclose(fl);
      } else {
        fprintf(stderr, "grep: %s: No such file or directory\n", files->line);
        exit(1);
      }
      files = files->next;
    }
  }
}

// читаем построчно файл
void read_lines(FILE *fl, struct options *opt, pattr *list, int cnt_files,
                char *file_name) {
  int cnt_if_c = 0, cnt_if_l = 0, cnt_line = 1, cnt_file_line = lines_file(fl);
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  fseek(fl, 0, SEEK_SET);
  while ((read = getline(&line, &len, fl)) != -1) {
    int count = compile_pattrn(opt, list, line);
    if (0 < count && (!opt->c && !opt->l && !opt->o)) {  // && !opt->o
      if (1 < cnt_files && !opt->h) {
        printf("%s:", file_name);
      }
      if (opt->n) {
        printf("%d:", cnt_line);
      }
      printf("%s", line);
    } else if (0 < count && (opt->c || opt->l)) {
      cnt_if_c++;
      cnt_if_l = 1;
    }
    if (0 > count && opt->v && !opt->c && !opt->o) {
      if (1 < cnt_files && !opt->h) {
        printf("%s:", file_name);
      }
      if (opt->n) {
        printf("%d:", cnt_line);
      }
      printf("ерш %s", line);
    }
    if (0 < count && opt->v && !opt->l && !opt->c) {
      print_last(count, cnt_line, cnt_file_line);
    }
    cnt_line++;
  }
  if (opt->c && opt->l) {
    flag_c(cnt_files, cnt_if_l, opt, file_name);
    flag_l(cnt_if_l, opt, file_name);
  } else if (opt->c && !opt->l) {
    flag_c(cnt_files, cnt_if_c, opt, file_name);
  } else if (opt->l && !opt->c) {
    flag_l(cnt_if_l, opt, file_name);
  }
  free(line);
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
      if (!opt->s) {
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[ind]);
      }
    }
    ind++;
  }
}