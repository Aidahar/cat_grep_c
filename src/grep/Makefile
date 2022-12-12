CC=gcc
CFLAGS=-Wall -Wextra -Werror -O2 -std=c11 -D_GNU_SOURCE
TEST=
all: build

build: s21_grep

s21_grep_test: s21_grep_test.o linked_list_test.o
	${CC} ${CFLAGS} ${TEST} s21_grep.o linked_list.o -o s21_grep `pcre-config --libs`

s21_grep: s21_grep.o linked_list.o
	${CC} ${CFLAGS} s21_grep.o linked_list.o -o s21_grep `pcre-config --libs`

s21_grep.o: s21_grep.c s21_grep.h
	${CC} ${CFLAGS} s21_grep.c s21_grep.h -c

s21_grep_test.o: s21_grep.c s21_grep.h
	${CC} ${CFLAGS} s21_grep.c s21_grep.h -c

linked_list.o: 
	${CC} ${CFLAGS} linked_list.c linked_list.h -c

linked_list_test.o: 
	${CC} ${CFLAGS} linked_list.c linked_list.h -c

.PHONY: all clean rebuild

rebuild: clean s21_grep 

rebuild_test: clean s21_grep_test

clean: 
	rm -rf *.o s21_grep *.gch *.dSYM