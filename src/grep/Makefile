CC=gcc
CFLAGS=-Wall -Wextra -Werror -O3 -std=c11
TEST=-fanalyzer -fsanitize=address -g -pedantic

all: build

build: s21_grep

s21_grep_test: s21_grep.o linked_list.o
	${CC} ${CFLAGS} ${TEST} s21_grep.o linked_list.o -o s21_grep `pcre-config --libs`

s21_grep: s21_grep.o linked_list.o
	${CC} ${CFLAGS} s21_grep.o linked_list.o -o s21_grep `pcre-config --libs`

s21_grep.o: s21_grep.c s21_grep.h
	${CC} ${CFLAGS} s21_grep.c s21_grep.h -c
linked_list.o: 
	${CC} ${CFLAGS} linked_list.c linked_list.h -c

.PHONY: all clean rebuild

rebuild: clean s21_grep 

clean: 
	rm -rf *.o s21_grep *.gch *.dSYM