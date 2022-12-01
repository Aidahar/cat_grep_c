#!/bin/bash

# Authored by Ruslan <medaelis>

#global vars
command_path=./s21_grep
SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
OPTS="s v n e i c l h f o"
NUMOPT=$(echo "$OPTS" | wc -w)
OPTLINE=""
PATTERN_FILE="pattern_file"
IFS=' ' readarray -t FILES < <(ls ./*.c ./*.h ./*.sh)
declare -a PATTERNS=(
  "for"
  "if"
  "while"
)
FILES+=("no_file")

# thx @johniety for some cases, cmp func & color msg
declare -a EXTRA=(
  "-n -e ^\} "
  "-c -e /\ "
  "-nivh = "
  #"-e" no stdin
  "-echar"
  "-noe ) "
  #"-c -e . -e '.'" #
  "-f nothing"
  "-e^if"
)

#funcctions
ex()
{
  # shellcheck disable=SC2086
  $command_path $1 $2 > test_s21_grep.log
  # shellcheck disable=SC2086
  grep $1 $2 > test_sys_grep.log
  PrintDiff "$1" "$2"
  rm test_s21_grep.log test_sys_grep.log
}


PrintDiff()
{
  DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
  (( COUNTER++ ))
  if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
  then
    (( SUCCESS++ ))
    echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $1 $2"
  else
    (( FAIL++ ))
    echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $1 $2"
  fi
}

ex_valgrind()
{
  # shellcheck disable=SC2086
  valgrind --log-file="valgrind_s21_grep.log" --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all $command_path $1 $2 > test_s21_grep.log
    leak=$(grep ERROR valgrind_s21_grep.log)
    (( COUNTER++ ))
    if [[ $leak == *"0 errors from 0 contexts"* ]]
    then
      (( SUCCESS++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $1 $2"
    else
      (( FAIL++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $1 $2"
#        echo "$leak"
    fi
    rm valgrind_s21_grep.log
    rm test_s21_grep.log
}

ChooseFiles()
{
  (( FILESN = RANDOM % ${#FILES[@]} + 1 ))
  FILE_STR=""
  for (( i = 0; i < FILESN; i++ )); do
    (( NUM = RANDOM % ${#FILES[@]} ))
    FILE_STR="$FILE_STR ${FILES[$NUM]}"
  done
  echo "$FILE_STR"
}

ChoosePattern()
{
  (( PATTERNN = RANDOM % ${#PATTERNS[@]} ))
    echo "${PATTERNS[PATTERNN]}"
}

test()
{
  if (( $1 == 0 )); then
    files=$(ChooseFiles)
    NEW_OPTLINE="${3/-e/"-e $(ChoosePattern)"}"
    NEW_OPTLINE="${NEW_OPTLINE/-f/"-f $PATTERN_FILE"}"
    if [ "$NEW_OPTLINE" = "$3" ] ; then
      NEW_OPTLINE="$NEW_OPTLINE $(ChoosePattern)"
    fi
    ex "$NEW_OPTLINE" "$files"
  else
    if (( $(echo "$2" | wc -w) >= $1 )) ; then
      local NEW_OPTS=$2
      for i in $2 ; do
        NEW_OPTS="${NEW_OPTS/$i/""}"
        NEW_OPTLINE="$3 -$i"
        test $(( $1 - 1 )) "$NEW_OPTS" "$NEW_OPTLINE"
      done
    fi
  fi
}

#main script

for (( NOPT = 0; NOPT <= NUMOPT; NOPT++ ))
do
    test "$NOPT" "$OPTS" "$OPTLINE"
done

for (( i = 0; i < ${#EXTRA[@]}; i++ )); do
  files=$(ChooseFiles)
  ex "${EXTRA[$i]}" "$files"
done

#vg test, no check for vg
if [ "$1" = "vg" ] ; then
  for (( i = 0; i < 5; i++ )); do
    files=$(ChooseFiles)
    ex "$files" "-sve for -l"
    ex "$files" "-sichf $PATTERN_FILE"
    ex "$files" "-snihf $PATTERN_FILE -o"
  done
fi