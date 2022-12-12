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
    if (re) {
      count = 0;
      int ovector[30];
      count =
          pcre_exec(re, NULL, (char *)line, strlen(line), 0, 0, ovector, 30);
      if (0 < count) {
        cnt_p_in_l++;
      }
    }
    pcre_free((void *)re);
  }
  count = check_l_v(cnt_p_in_l, opt);

  return count;
}

int check_l_v(int cnt_p_in_l, struct options *opt) {
  int count = 0;
  if (cnt_p_in_l && opt->v) {
    count = 0;
  } else if (!cnt_p_in_l && opt->v) {
    count = 1;
  } else if (cnt_p_in_l) {
    count = 1;
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

void flag_o(struct options *opt, char *line, pattr *list) {
  pcre *re;
  int options = 0, cursor = 0;
  int erroffset;
  const char *error;
  int len = strlen(line);
  char estr[len];
  for (int i = 0; i < len; i++) {
    estr[i] = line[i];
  }
  estr[len] = '\0';
  for (; list != NULL; list = list->next) {
    char *print_pat = malloc(sizeof(char) * (strlen(list->line) + 1));
    if (opt->i) {
      re = pcre_compile((char *)list->line, PCRE_CASELESS, &error, &erroffset,
                        NULL);
    } else {
      re = pcre_compile((char *)list->line, options, &error, &erroffset, NULL);
    }
    if (re) {
      int ovector[30];
      int idx_s;
      int idx_end;
      int count = 1;
      while (cursor < (int)strlen(estr) && 0 < count) {
        count = pcre_exec(re, NULL, line, strlen(line), 0, 0, ovector, 30);
        if (0 < count) {
          int i = 0;
          idx_s = ovector[0];
          idx_end = ovector[1];
          for (; idx_s < idx_end; idx_s++) {
            print_pat[i++] = line[idx_s];
          }
          print_pat[i] = '\0';
          printf("%s\n", print_pat);
          cursor += idx_end;
          line += idx_end;
        }
      }
    }
    free(print_pat);
    pcre_free((void *)re);
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
  release(files);
  release(list);
}

void print_last(int count, int cnt_line, int cnt_file_line) {
  if (0 < count && cnt_line == cnt_file_line) printf("\n");
}

// читаем из файла паттернов в лист
void read_from_f(pattr **pat, pattr *files) {
  while (files) {
    if (255 < strlen(files->line)) {
      release(files);
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
          free(patr);
        }
        free(l);
        fclose(fl);
      } else {
        fprintf(stderr, "grep: %s: No such file or directory\n", files->line);
        release(files);
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
  if (opt->v) opt->o = 0;
  while ((read = getline(&line, &len, fl)) != -1) {
    int count = compile_pattrn(opt, list, line);
    if (0 < count && !(opt->c || opt->l)) {
      if (1 < cnt_files && !opt->h) {
        printf("%s:", file_name);
      }
      if (opt->n) {
        printf("%d:", cnt_line);
      }
      if ((opt->v && opt->o) || !opt->o) {
        printf("%s", line);
      } else {
        flag_o(opt, line, list);
      }
    } else if (0 < count) {
      cnt_if_c++;
      cnt_if_l = 1;
    }
    if (0 < count && !opt->l && !opt->c && line[read - 1] != '\n' &&
        (!opt->o || opt->v)) {
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
