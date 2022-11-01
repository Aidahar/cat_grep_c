#include "s21_cat.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        int flag = file_open(argv[1]);
        if (0 == flag) {
            printf("Cant read file");
        }
    }
    return 0;
}

static int file_open(const char *file_name) {
    int flag = 1;
    FILE *file = fopen(file_name, "r");
    if (NULL == file) {
        flag = 0;
    } else {
        read_line(file);
    }
    fclose(file);
    return flag;
}

static void read_line(FILE *file) {
    int c = 1, b = 3;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
        if (0 == b) {
            print_n(c, line);
            c++;
        } else if (1 == b) {
            print_e(line);
        } else if (2 == b) {
            print_t(line);
        } else if (3 == b) {
            print_b(&c, line);
        }
    }
    if (line) {
        free(line);
    }
}

int cnt_symbol(char *str) {
    int i = 0;
    while ('\0' != str[i]) {
        i++;
    }
    return i;
}

static void print_b(int *c, const char *line) { 
    if ('\n' != *line) {
        printf("%6d\t%s", *c, line);
        *c += 1;
    }
}

static void print_n(int c, const char *line) { printf("%6d\t%s", c, line); }

static void print_e(char *line) {
    int cnt = cnt_symbol(line);
    if ('\n' == line[cnt - 1]) {
        line[cnt - 1] = '$';
    }
    printf("%s\n", line);
}

static void print_t(char *line) {
    while ('\0' != *line) {
        if (*line == '\t') {
            *line = '^';
            printf("%cI", *line);
            line++;
        } else {
            printf("%c", *line);
            line++;
        }
    }
}
