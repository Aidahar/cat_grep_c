# MAC(LEAKS): // NE NADEZHNO :)
# -----------
leakmacro () {
  leaks -quiet -atExit -- $1 $2 $3 $4 $5 $6 $7 $8 $9 > leak.log
  grep -e Process leak.log
}

# LINUX(VALGRIND):
# ----------------
# leakmacro () {
#   valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -q $1 $2 $3 $4 $5 $6 $7 $8 $9 > leak.log
#   grep -i errors leak.log 
# }

filename1=diffgrep.sh;
filename2=s21_grep.c;
# filename3 is also used for -f flag patterns:
filename3=s21_grep.h;

echo ----------------------------------------------*GREP LEAK CHECK 1
leakmacro ./s21_grep int "$filename2" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 1
leakmacro ./s21_grep -e int "$filename2" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 2
leakmacro ./s21_grep -i int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 3
leakmacro ./s21_grep -v int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 4
leakmacro ./s21_grep -c int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 5
leakmacro ./s21_grep -l int "$filename2" "$filename3" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 6
leakmacro ./s21_grep -n int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 7 BONUS
leakmacro ./s21_grep -h int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 8 BONUS
leakmacro ./s21_grep -o int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 9 BONUS
leakmacro ./s21_grep -h int "$filename2" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 10 BONUS
leakmacro ./s21_grep int -s aboba
echo ----------------------------------------------*GREP LEAK CHECK 11 BONUS
leakmacro ./s21_grep -f "$filename3" "$filename2"
echo ----------------------------------------------*GREP LEAK CHECK 12 BONUS
leakmacro ./s21_grep -in int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 13 BONUS
leakmacro ./s21_grep -cv int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 14 BONUS
leakmacro ./s21_grep -iv int "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 15 BONUS
leakmacro ./s21_grep -lv int "$filename2" "$filename3" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 16 BONUS
leakmacro ./s21_grep -ho int "$filename2" "$filename1"
echo ----------------------------------------------*GREP LEAK CHECK 17 BONUS
leakmacro ./s21_grep -nf "$filename3" "$filename2"
echo ----------------------------------------------*Leak tests ended*
