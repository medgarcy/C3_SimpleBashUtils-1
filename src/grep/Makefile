FLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -ggdb

all: s21_grep

s21_grep: s21_grep.c
	gcc $(FLAGS) s21_grep.c -o s21_grep

test: s21_grep
	bash test/test.sh

test2: s21_grep
	bash test.bash

check: 
	clang-format -style=Google -n *.c *.h

checkdo: 
	clang-format -style=Google -i *.c *.h

rebuid:
	make clean
	make all

clean:
	rm -f s21_grep