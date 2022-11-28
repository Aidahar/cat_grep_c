#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE

struct options {
    unsigned char b : 1;
    unsigned char e : 1;
    unsigned char n : 1;
    unsigned char s : 1;
    unsigned char t : 1;
    unsigned char v : 1;
};

int cnt_symbol(char *str);
void parser_flag(int argc, char **argv, struct options *opt);
static void print_b(int *c, const char *line);
static void print_e(char *line);
static void print_n(int c, const char *line);
static void print_t(char *line);
void reader(char **argv, struct options *opt);

int main(int argc, char **argv) {
    struct options opt = {0};
    parser_flag(argc, argv, &opt);
    if (opt.b == '1') {
        opt.n = 0;
    }
    reader(argv, &opt);
    return 0;
}

void parser_flag(int argc, char **argv, struct options *opt) {
    int rez = 0, long_opt = 0;
    const char *short_options = "+benstvTE"; // если стоит плюс он перестает считывать опции как только встретил файл
    static struct option long_options[] = {
        { "number-nonblank", 0, NULL, 'b'},
		{ "number", 0, NULL, 'n'},
		{ "squeeze-blank", 0, NULL, 's'},
		{ NULL, 0, NULL, 0 }
    };
    while (-1 != (rez = getopt_long(argc, argv, short_options, long_options, &long_opt))) {
        switch(rez) {
            case 'b':
                opt->b = 1;
                break;
            case 'e':
                opt->e = 1;
                opt->v = 1;
                break;
            case 'E':
                opt->e = 1;
                opt->v = 1;
                break;
            case 'n':
                opt->n = 1;
                break;
            case 's':
                opt->s = 1;
                break;
            case 't':
                opt->t = 1;
                opt->v = 1;
                break;
            case 'T':
                opt->t = 1;
                opt->v = 1;
                break;
            default:
                fprintf(stderr, "usage: [-benstv] [file ...]");
                exit(1);
        }
    }
}

void reader(char **argv, struct options *opt) {
    FILE *file = fopen(argv[optind], "r"); // после считывания оптаргом в оптинд записывается следующий файл
    if (file) {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int c = 1;
        while ((read = getline(&line, &len, file)) != -1) {
            if (opt->b) {
                print_b(&c, line);
            }
            if (opt->e) {
                print_e(line);
            }
            if (opt->n) {
                print_n(c, line);
                c++;
            }
            if (opt->t) {
                print_t(line);
            }
        }
        if (line) {
            free(line);
        }        
    } else {
        fprintf(stderr, "no such file");
    }
}

int cnt_symbol(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

static void print_b(int *c, const char *line) { 
    if ('\n' != *line) {
        printf("%6d\t%s", *c, line);
        *c += 1;
    } else {
        printf("\n");
    }
}

static void print_e(char *line) {
    int cnt = cnt_symbol(line);
    if (line[cnt - 1] == '\n') {
        line[cnt - 1] = '$';
    }
    printf("%s\n", line);
}

static void print_n(int c, const char *line) { printf("%6d\t%s", c, line); }

static void print_t(char *line) {
    while ('\0' != *line) {
        if (*line == '\t') {
            printf("^I");
            line++;
        } else {
            printf("%c", *line);
            line++;
        }
    }
}