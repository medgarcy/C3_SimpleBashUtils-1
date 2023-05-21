FLAGS = -Wall -Wextra -Werror -std=c11 -pedantic

all: s21_cat

s21_cat: s21_cat.c
	gcc $(FLAGS) s21_cat.c catFlags.c -o s21_cat

test: s21_cat
	bash test.bash

rebuild:
	make clean
	make all

clean:
	rm -f s21_cat